/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

    This file is part of Kronometer.

    Kronometer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Kronometer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Kronometer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lapmodel.h"
#include "mainwindow.h"
#include "sessiondialog.h"
#include "sessionmodel.h"
#include "settings.h"
#include "stopwatch.h"
#include "timedisplay.h"
#include "widgets/colorsettings.h"
#include "widgets/fontsettings.h"
#include "widgets/generalsettings.h"
#include "widgets/guisettings.h"
#include "widgets/savesettings.h"

#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPointer>
#include <QSaveFile>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QStatusBar>
#include <QTableView>

namespace
{
    const QString START_KEY = "start";
    const QString PAUSE_KEY = "pause";
    const QString RESET_KEY = "reset";
    const QString LAP_KEY = "lap";
    const QString EXPORT_KEY = "export_laps";

    const QString QT_PLACE_HOLDER = "[*]";           /** Qt standard placeholder for setWindowModified() */

    const QString JSON_MIMETYPE = "application/json";
    const QString CSV_MIMETYPE = "text/csv";
    const QString JSON_EXTENSION = ".json";
    const QString CSV_EXTENSION = ".csv";

    // kronometerui.rc states
    const QString INACTIVE_STATE = "inactive";
    const QString RUNNING_STATE = "running";
    const QString PAUSED_STATE = "paused";
    const QString PAUSED_SESSION_STATE = "pausedSession";  /** An open file has been paused */
}

MainWindow::MainWindow(QWidget *parent, const Session& session) : KXmlGuiWindow(parent), m_session(session)
{
    stopwatch = new Stopwatch(this);
    stopwatchDisplay = new TimeDisplay(this);
    connect(stopwatch, &Stopwatch::time, stopwatchDisplay, &TimeDisplay::onTime);  // bind stopwatch to its display

    m_sessionModel = new SessionModel(this);

    setupCentralWidget();
    setupStatusBar();
    setupActions();
    loadSettings();

    if (not m_session.isEmpty()) {
        loadSession();
    }
    else {
        setWindowTitle(i18n("Untitled") + QT_PLACE_HOLDER);
    }
}

bool MainWindow::queryClose()
{
    if (stopwatch->isInactive()) {
        return true;  // exit without ask
    }

    if (stopwatch->isRunning()) {
        stopwatch->onPause();
        slotPaused();
    }

    KMessageBox::ButtonCode buttonCode;

    if (m_session.isEmpty()) {
        buttonCode = KMessageBox::warningContinueCancel(
                    this,
                    i18n("Do you want to quit and lose your unsaved times?"),
                    i18n("Confirm quit"),
                    KStandardGuiItem::quit(),
                    KStandardGuiItem::cancel(),
                    "quit-and-lose-times");

        if (buttonCode != KMessageBox::Cancel) {
            return true;
        }
        return false;
    }
    else if (m_session.isOutdated()) {
        buttonCode = KMessageBox::warningYesNoCancel(this, i18n("Save times to session %1?", m_session.name()));

        switch (buttonCode) {
        case KMessageBox::Yes:
            slotSaveSession();
            return true;
        case KMessageBox::No:
            return true;
        default: // cancel
            return false;
        }
    }

    return true;  // there is an open session, but times are already saved.
}

void MainWindow::slotRunning()
{
    statusLabel->setText(i18n("Running..."));

    m_session.setOutdated(true);
    setWindowModified(true);

    stateChanged(RUNNING_STATE);
}

void MainWindow::slotPaused()
{
    startAction->setText(i18n("Re&sume"));
    statusLabel->setText(i18n("Paused"));

    if (not m_session.isEmpty()) {
        stateChanged(PAUSED_SESSION_STATE);
    }
    else {
        stateChanged(PAUSED_STATE);
    }

    // the export action can be used only if there are laps (in both the paused states).
    // so, it can't be enabled directly from kronometerui.rc
    if (not lapModel->isEmpty()) {
        exportAction->setEnabled(true);
    }
}

void MainWindow::slotInactive()
{
    startAction->setText(i18n("&Start"));
    statusLabel->setText(i18n("Inactive"));

    m_session.setOutdated(false);
    setWindowModified(false);

    stateChanged(INACTIVE_STATE);
}

void MainWindow::slotShowSettings()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(this, "settings", KronometerConfig::self());

    KPageWidgetItem *generalPage = dialog->addPage(new GeneralSettings(this), i18n("General settings"));
    generalPage->setIcon(QIcon::fromTheme(QApplication::windowIcon().name()));

    KPageWidgetItem *fontPage = dialog->addPage(new FontSettings(this), i18n("Font settings"));
    fontPage->setIcon(QIcon::fromTheme("preferences-desktop-font"));

    KPageWidgetItem *colorPage = dialog->addPage(new ColorSettings(this), i18n("Color settings"));
    colorPage->setIcon(QIcon::fromTheme("fill-color"));

    KPageWidgetItem *guiPage = dialog->addPage(new GuiSettings(this), i18n("Interface settings"));
    guiPage->setIcon(QIcon::fromTheme("preferences-desktop-theme"));

    KPageWidgetItem *savePage = dialog->addPage(new SaveSettings(this), i18n("Save settings"));
    savePage->setIcon(QIcon::fromTheme("document-save"));

    connect(dialog, &KConfigDialog::settingsChanged, this, &MainWindow::slotWriteSettings);

    dialog->show();
}

void MainWindow::slotWriteSettings(const QString& dialogName)
{
    Q_UNUSED(dialogName)

    KronometerConfig::self()->save();

    MainWindow *window = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        window = qobject_cast<MainWindow *>(widget);

        if (window) {
            window->loadSettings();
        }
    }
}

void MainWindow::slotUpdateLapDock()
{
    lapView->resizeColumnsToContents();
    lapView->horizontalHeader()->setStretchLastSection(true);
    lapView->selectRow(lapModel->rowCount(QModelIndex()) - 1);  // rows indexes start from 0
}

void MainWindow::slotNewSession()
{
    MainWindow *window = new MainWindow();
    window->show();
}

void MainWindow::slotOpenSession()
{
    QPointer<SessionDialog> dialog = new SessionDialog(nullptr, i18n("Sessions"));
    dialog.data()->show();

    if (dialog.data()->exec() == QDialog::Accepted) {
        MainWindow *window = new MainWindow(nullptr, dialog.data()->selectedSession());
        window->show();
    }

    delete dialog.data();
}

void MainWindow::slotSaveSession()
{
    if (not m_session.isOutdated())
        return;

    m_session.clear();    // required for laps consistency
    m_session.setTime(stopwatch->raw());

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        m_session.addLap(lapModel->at(i));
    }

    m_session.setOutdated(false);
    m_sessionModel->update(m_session);

    setWindowModified(false);
}

void MainWindow::slotSaveSessionAs()
{
    QString name = QInputDialog::getText(this, i18n("Choose a name"), i18n("Session name:"));

    if (name.isEmpty())
        name = i18n("Untitled session");

    slotSaveSessionAs(name);
}

void MainWindow::slotExportLapsAs()
{
    QPointer<QFileDialog> dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setConfirmOverwrite(true);
    dialog->setWindowTitle(i18n("Choose export file destination"));

    QStringList mimeTypes;
    mimeTypes << CSV_MIMETYPE << JSON_MIMETYPE;
    dialog->setMimeTypeFilters(mimeTypes);

    if (dialog->exec() == QDialog::Accepted) {
        exportLapsAs(dialog->selectedFiles().first(), dialog->selectedNameFilter());
    }

    delete dialog;
}

void MainWindow::slotCopyToClipboard()
{
    QApplication::clipboard()->setText(stopwatchDisplay->currentTime());
}

void MainWindow::setupCentralWidget()
{
    centralSplitter = new QSplitter(this);

    lapModel = new LapModel(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(lapModel);

    lapView = new QTableView(this);
    lapView->setModel(proxyModel);
    lapView->setSelectionBehavior(QAbstractItemView::SelectRows);
    lapView->setGridStyle(Qt::DotLine);
    lapView->verticalHeader()->hide();
    lapView->resizeColumnsToContents();
    lapView->horizontalHeader()->setStretchLastSection(true);
    lapView->setSortingEnabled(true);
    lapView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);

    centralSplitter->setOrientation(Qt::Horizontal);
    centralSplitter->setChildrenCollapsible(false);
    centralSplitter->addWidget(stopwatchDisplay);
    centralSplitter->addWidget(lapView);

    setCentralWidget(centralSplitter);
}

void MainWindow::setupStatusBar()
{
    statusLabel = new QLabel(this);
    statusLabel->setToolTip(i18n("Current chronometer status"));

    statusBar()->addWidget(statusLabel);
}

void MainWindow::setupActions()
{
    startAction = new QAction(this);
    pauseAction = new QAction(this);
    resetAction = new QAction(this);
    lapAction = new QAction(this);
    exportAction = new QAction(this);

    startAction->setIcon(QIcon::fromTheme("player-time"));

    pauseAction->setText(i18n("&Pause"));  // pauseAction/resetAction have fixed text (startAction doesn't)
    pauseAction->setIcon(QIcon::fromTheme("media-playback-pause"));

    resetAction->setText(i18n("&Reset"));
    resetAction->setIcon(QIcon::fromTheme("edit-clear-history"));

    lapAction->setText(i18n("&Lap"));
    lapAction->setIcon(QIcon::fromTheme("chronometer"));

    exportAction->setText(i18n("&Export laps as..."));
    exportAction->setIcon(QIcon::fromTheme("document-export"));

    actionCollection()->addAction(START_KEY, startAction);
    actionCollection()->addAction(PAUSE_KEY, pauseAction);
    actionCollection()->addAction(RESET_KEY, resetAction);
    actionCollection()->addAction(LAP_KEY, lapAction);
    actionCollection()->addAction(EXPORT_KEY, exportAction);
    actionCollection()->setDefaultShortcut(startAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(pauseAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(resetAction, Qt::Key_F5);
    actionCollection()->setDefaultShortcut(lapAction, Qt::Key_Return);

    // triggers for Stopwatch "behavioral" slots
    connect(startAction, &QAction::triggered, stopwatch, &Stopwatch::onStart);
    connect(pauseAction, &QAction::triggered, stopwatch, &Stopwatch::onPause);
    connect(resetAction, &QAction::triggered, stopwatch, &Stopwatch::onReset);
    connect(lapAction, &QAction::triggered, stopwatch, &Stopwatch::onLap);

    // triggers for LapModel slots
    connect(resetAction, &QAction::triggered, lapModel,&LapModel::onClear);
    connect(stopwatch, &Stopwatch::lap, lapModel, &LapModel::onLap);

    // triggers for MainWindow "gui" slots
    connect(startAction, &QAction::triggered, this, &MainWindow::slotRunning);
    connect(pauseAction, &QAction::triggered, this, &MainWindow::slotPaused);
    connect(resetAction, &QAction::triggered, this, &MainWindow::slotInactive);
    connect(lapAction, &QAction::triggered, this, &MainWindow::slotUpdateLapDock);

    // File menu triggers
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(slotShowSettings()), actionCollection());
    KStandardAction::openNew(this, SLOT(slotNewSession()), actionCollection());
    KStandardAction::save(this, SLOT(slotSaveSession()), actionCollection());
    KStandardAction::saveAs(this, SLOT(slotSaveSessionAs()), actionCollection());
    KStandardAction::open(this, SLOT(slotOpenSession()), actionCollection());
    KStandardAction::copy(this, SLOT(slotCopyToClipboard()), actionCollection());
    connect(exportAction, &QAction::triggered, this, &MainWindow::slotExportLapsAs);

    setupGUI(Default, "kronometerui.rc");

    slotInactive();	// inactive state is the default
}


void MainWindow::loadSettings()
{
    TimeFormat timeFormat(
        KronometerConfig::showHours(),
        KronometerConfig::showMinutes(),
        KronometerConfig::showSeconds(),
        KronometerConfig::showTenths(),
        KronometerConfig::showHundredths(),
        KronometerConfig::showMilliseconds()
    );
    TimeFormat lapTimeFormat(
        KronometerConfig::showLapHours(),
        KronometerConfig::showLapMinutes(),
        KronometerConfig::showLapSeconds(),
        KronometerConfig::showLapTenths(),
        KronometerConfig::showLapHundredths(),
        KronometerConfig::showLapMilliseconds()
    );

    lapAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    exportAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    lapView->setVisible(KronometerConfig::isLapsRecordingEnabled());
    lapModel->setTimeFormat(lapTimeFormat);
    timeFormat.showDividers(false);
    stopwatchDisplay->setTimeFormat(timeFormat);
    stopwatchDisplay->setHourFont(KronometerConfig::hourFont());
    stopwatchDisplay->setMinFont(KronometerConfig::minFont());
    stopwatchDisplay->setSecFont(KronometerConfig::secFont());
    stopwatchDisplay->setFracFont(KronometerConfig::fracFont());
    stopwatchDisplay->setBackgroundColor(KronometerConfig::backgroundColor());
    stopwatchDisplay->setTextColor(KronometerConfig::textColor());
    stopwatchDisplay->showHeaders(KronometerConfig::showTimeHeaders());

    setupGranularity(KronometerConfig::showTenths(), KronometerConfig::showHundredths(), KronometerConfig::showMilliseconds());
}


void MainWindow::setupGranularity(bool tenths, bool hundredths, bool msec)
{
    if (msec) {
        stopwatch->setGranularity(Stopwatch::MILLISECONDS);
    }
    else if (hundredths) {
        stopwatch->setGranularity(Stopwatch::HUNDREDTHS);
    }
    else if (tenths) {
        stopwatch->setGranularity(Stopwatch::TENTHS);
    }
    else {
        stopwatch->setGranularity(Stopwatch::SECONDS);
    }
}

void MainWindow::slotSaveSessionAs(const QString& name)
{
    Session newSession(stopwatch->raw());
    newSession.setName(name);

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        newSession.addLap(lapModel->at(i));
    }

    m_sessionModel->append(newSession);
    m_sessionModel->write();

    m_session = newSession;

    setWindowTitle(m_session.name() + QT_PLACE_HOLDER);
    setWindowModified(false);
}

void MainWindow::loadSession()
{
    stopwatch->initialize(m_session.time());

    Q_FOREACH (const Lap& lap, m_session.laps()) {
        lapModel->append(lap);
    }

    slotPaused();   // enter in paused state
    setWindowTitle(m_session.name() + QT_PLACE_HOLDER);
}

void MainWindow::exportLapsAs(const QString& name, const QString& nameFilter)
{
    if (name.isEmpty()) {
        return;
    }

    QString exportName = name;

    if (nameFilter.contains(JSON_EXTENSION)) {
        if (not exportName.endsWith(JSON_EXTENSION)) {
            exportName += JSON_EXTENSION;
        }

        QSaveFile exportFile(exportName);
        exportFile.open(QIODevice::WriteOnly);

        QJsonObject json;
        exportLapsAsJson(json);

        QJsonDocument exportDoc(json);
        exportFile.write(exportDoc.toJson());
        exportFile.commit();
    }
    else if (nameFilter.contains(CSV_EXTENSION)) {
        if (not exportName.endsWith(CSV_EXTENSION)) {
            exportName += CSV_EXTENSION;
        }

        QSaveFile exportFile(exportName);
        exportFile.open(QIODevice::WriteOnly);

        QTextStream stream(&exportFile);
        exportLapsAsCsv(stream);

        exportFile.commit();
    }
}

void MainWindow::exportLapsAsJson(QJsonObject& json)
{
    QJsonArray laps;

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        QJsonObject object;
        lapModel->at(i).write(object);
        laps.append(object);
    }

    json["laps"] = laps;
}

void MainWindow::exportLapsAsCsv(QTextStream& out)
{
    out << '#' << timestampMessage() << '\r' << '\n';
    out << '#' << i18n("Lap number,Lap time,Global time,Note") << '\r' << '\n';

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        out << i;
        out << ',' << lapModel->at(i).relativeTime();
        out << ',' << lapModel->at(i).absoluteTime();
        out << ',' << lapModel->at(i).note();
        out << '\r' << '\n';
    }
}

QString MainWindow::timestampMessage()
{
    QDateTime timestamp = QDateTime::currentDateTime();

    return i18n("Created by Kronometer on %1", timestamp.toString(Qt::DefaultLocaleLongDate));
}

