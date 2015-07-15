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

#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDBusConnection>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QSaveFile>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QStatusBar>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent, const Session& session) : KXmlGuiWindow(parent), m_session(session)
{
    m_stopwatch = new Stopwatch(this);
    m_stopwatchDisplay = new TimeDisplay(this);
    connect(m_stopwatch, &Stopwatch::time, m_stopwatchDisplay, &TimeDisplay::slotTime);  // bind stopwatch to its display

    m_sessionModel = new SessionModel(this);

    setupCentralWidget();
    setupStatusBar();
    setupActions();
    loadSettings();

    if (not m_session.isEmpty()) {
        loadSession();
    }
    else {
        setWindowTitle(i18n("Untitled"));
    }

    // TODO: replace this whit solid-power API, once it's released.
    QDBusConnection::systemBus().connect(
                QStringLiteral("org.freedesktop.login1"),
                QStringLiteral("/org/freedesktop/login1"),
                QStringLiteral("org.freedesktop.login1.Manager"),
                QStringLiteral("PrepareForSleep"),
                this, SLOT(slotPrepareForSleep(bool)));
}

void MainWindow::setWindowTitle(const QString& title)
{
    if (title.endsWith(QLatin1String("[*]"))) {
        KXmlGuiWindow::setWindowTitle(title);
    }
    else {
        KXmlGuiWindow::setWindowTitle(title + QLatin1String("[*]"));
    }
}

bool MainWindow::queryClose()
{
    if (m_stopwatch->isInactive()) {
        return true;  // exit without ask
    }

    if (m_stopwatch->isRunning()) {
        m_stopwatch->slotPause();
        slotPaused();
    }

    if (m_session.isEmpty()) {
        auto buttonCode = KMessageBox::warningContinueCancel(
                    this,
                    i18n("Do you want to quit and lose your unsaved times?"),
                    i18n("Confirm quit"),
                    KStandardGuiItem::quit(),
                    KStandardGuiItem::cancel(),
                    QStringLiteral("quit-and-lose-times"));

        if (buttonCode != KMessageBox::Cancel) {
            return true;
        }
        return false;
    }
    else if (m_session.isOutdated()) {
        auto buttonCode = KMessageBox::warningYesNoCancel(this, i18n("Save times to session %1?", m_session.name()));

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
    m_statusLabel->setText(i18n("Running..."));

    m_session.setOutdated(true);
    setWindowModified(true);

    stateChanged(QStringLiteral("running"));
}

void MainWindow::slotPaused()
{
    m_startAction->setText(i18n("Re&sume"));
    m_statusLabel->setText(i18n("Paused"));

    if (not m_session.isEmpty()) {
        stateChanged(QStringLiteral("pausedSession"));
    }
    else {
        stateChanged(QStringLiteral("paused"));
    }

    // the export action can be used only if there are laps (in both the paused states).
    // so, it can't be enabled directly from kronometerui.rc
    if (not m_lapModel->isEmpty()) {
        m_exportAction->setEnabled(true);
    }
}

void MainWindow::slotInactive()
{
    m_startAction->setText(i18n("&Start"));
    m_statusLabel->setText(i18n("Inactive"));

    m_session.setOutdated(false);

    setWindowTitle(i18n("Untitled"));
    setWindowModified(false);

    stateChanged(QStringLiteral("inactive"));
}

void MainWindow::slotPrepareForSleep(bool beforeSleep)
{
    if (not beforeSleep)
        return;

    m_stopwatch->slotPause();
    slotPaused();
}

void MainWindow::slotShowSettings()
{
    if (KConfigDialog::showDialog(QLatin1String("settings"))) {
        return;
    }

    auto dialog = new KConfigDialog(this, QLatin1String("settings"), KronometerConfig::self());

    auto generalPage = dialog->addPage(new GeneralSettings(this), i18n("General settings"));
    generalPage->setIcon(QIcon::fromTheme(QApplication::windowIcon().name()));

    auto fontPage = dialog->addPage(new FontSettings(this), i18n("Font settings"));
    fontPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));

    auto colorPage = dialog->addPage(new ColorSettings(this), i18n("Color settings"));
    colorPage->setIcon(QIcon::fromTheme(QStringLiteral("fill-color")));

    auto guiPage = dialog->addPage(new GuiSettings(this), i18n("Interface settings"));
    guiPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-theme")));

    connect(dialog, &KConfigDialog::settingsChanged, this, &MainWindow::slotWriteSettings);

    dialog->show();
}

void MainWindow::slotWriteSettings()
{
    KronometerConfig::self()->save();

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        auto window = qobject_cast<MainWindow*>(widget);

        if (window) {
            window->loadSettings();
        }
    }
}

void MainWindow::slotUpdateLapDock()
{
    m_lapView->resizeColumnsToContents();
    m_lapView->horizontalHeader()->setStretchLastSection(true);
    m_lapView->selectRow(m_lapModel->rowCount(QModelIndex()) - 1);  // rows indexes start from 0
}

void MainWindow::slotNewSession()
{
    auto window = new MainWindow();
    window->show();
}

void MainWindow::slotOpenSession()
{
    QPointer<SessionDialog> dialog = new SessionDialog(nullptr, i18n("Sessions"));

    if (dialog.data()->exec() == QDialog::Accepted) {
        auto window = new MainWindow(nullptr, dialog.data()->selectedSession());
        window->show();
    }

    delete dialog.data();
}

void MainWindow::slotSaveSession()
{
    if (not m_session.isOutdated())
        return;

    m_session.clear();    // required for laps consistency
    m_session.setTime(m_stopwatch->raw());

    for (int i = 0; i < m_lapModel->rowCount(QModelIndex()); i++) {
        m_session.addLap(m_lapModel->at(i));
    }

    m_session.setOutdated(false);
    m_sessionModel->update(m_session);

    setWindowModified(false);
}

void MainWindow::slotSaveSessionAs()
{
    auto name = QInputDialog::getText(this, i18n("Choose a name"), i18n("Session name:"));

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

    auto mimeTypes = { QStringLiteral("text/csv"), QStringLiteral("application/json") };
    dialog->setMimeTypeFilters(mimeTypes);

    if (dialog->exec() == QDialog::Accepted) {
        exportLapsAs(dialog->selectedFiles().first(), dialog->selectedNameFilter());
    }

    delete dialog;
}

void MainWindow::slotCopyToClipboard()
{
    QApplication::clipboard()->setText(m_stopwatchDisplay->currentTime());
}

void MainWindow::setupCentralWidget()
{
    m_centralSplitter = new QSplitter(this);

    m_lapModel = new LapModel(this);
    auto proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(m_lapModel);

    m_lapView = new QTableView(this);
    m_lapView->setModel(proxyModel);
    m_lapView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_lapView->setGridStyle(Qt::DotLine);
    m_lapView->verticalHeader()->hide();
    m_lapView->resizeColumnsToContents();
    m_lapView->horizontalHeader()->setStretchLastSection(true);
    m_lapView->setSortingEnabled(true);
    m_lapView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);

    m_centralSplitter->setOrientation(Qt::Horizontal);
    m_centralSplitter->setChildrenCollapsible(false);
    m_centralSplitter->addWidget(m_stopwatchDisplay);
    m_centralSplitter->addWidget(m_lapView);

    setCentralWidget(m_centralSplitter);
}

void MainWindow::setupStatusBar()
{
    m_statusLabel = new QLabel(this);
    m_statusLabel->setToolTip(i18n("Current chronometer status"));

    statusBar()->addWidget(m_statusLabel);
}

void MainWindow::setupActions()
{
    m_startAction = new QAction(this);
    m_pauseAction = new QAction(this);
    m_resetAction = new QAction(this);
    m_lapAction = new QAction(this);
    m_exportAction = new QAction(this);

    m_startAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-start")));

    m_pauseAction->setText(i18n("&Pause"));  // pauseAction/resetAction have fixed text (startAction doesn't)
    m_pauseAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-pause")));

    m_resetAction->setText(i18n("&Reset"));
    m_resetAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-reset")));

    m_lapAction->setText(i18n("&Lap"));
    m_lapAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-lap")));

    m_exportAction->setText(i18n("&Export laps as..."));
    m_exportAction->setIcon(QIcon::fromTheme(QStringLiteral("document-export")));

    actionCollection()->addAction(QStringLiteral("start"), m_startAction);
    actionCollection()->addAction(QStringLiteral("pause"), m_pauseAction);
    actionCollection()->addAction(QStringLiteral("reset"), m_resetAction);
    actionCollection()->addAction(QStringLiteral("lap"), m_lapAction);
    actionCollection()->addAction(QStringLiteral("export_laps"), m_exportAction);
    actionCollection()->setDefaultShortcut(m_startAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(m_pauseAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(m_resetAction, Qt::Key_F5);
    actionCollection()->setDefaultShortcut(m_lapAction, Qt::Key_Return);

    // triggers for Stopwatch "behavioral" slots
    connect(m_startAction, &QAction::triggered, m_stopwatch, &Stopwatch::slotStart);
    connect(m_pauseAction, &QAction::triggered, m_stopwatch, &Stopwatch::slotPause);
    connect(m_resetAction, &QAction::triggered, m_stopwatch, &Stopwatch::slotReset);
    connect(m_lapAction, &QAction::triggered, m_stopwatch, &Stopwatch::slotLap);

    // triggers for LapModel slots
    connect(m_resetAction, &QAction::triggered, m_lapModel,&LapModel::slotClear);
    connect(m_stopwatch, &Stopwatch::lap, m_lapModel, &LapModel::slotLap);

    // triggers for MainWindow "gui" slots
    connect(m_startAction, &QAction::triggered, this, &MainWindow::slotRunning);
    connect(m_pauseAction, &QAction::triggered, this, &MainWindow::slotPaused);
    connect(m_resetAction, &QAction::triggered, this, &MainWindow::slotInactive);
    connect(m_lapAction, &QAction::triggered, this, &MainWindow::slotUpdateLapDock);

    // File menu triggers
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(slotShowSettings()), actionCollection());
    KStandardAction::openNew(this, SLOT(slotNewSession()), actionCollection());
    KStandardAction::save(this, SLOT(slotSaveSession()), actionCollection());
    KStandardAction::saveAs(this, SLOT(slotSaveSessionAs()), actionCollection());
    KStandardAction::open(this, SLOT(slotOpenSession()), actionCollection());
    KStandardAction::copy(this, SLOT(slotCopyToClipboard()), actionCollection());
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::slotExportLapsAs);

    setupGUI(Default, QStringLiteral("kronometerui.rc"));

    slotInactive();    // inactive state is the default
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

    m_lapAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_exportAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_lapView->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_lapModel->setTimeFormat(lapTimeFormat);
    timeFormat.showDividers(false);
    m_stopwatchDisplay->setTimeFormat(timeFormat);
    m_stopwatchDisplay->setHourFont(KronometerConfig::hourFont());
    m_stopwatchDisplay->setMinFont(KronometerConfig::minFont());
    m_stopwatchDisplay->setSecFont(KronometerConfig::secFont());
    m_stopwatchDisplay->setFracFont(KronometerConfig::fracFont());
    m_stopwatchDisplay->setBackgroundColor(KronometerConfig::backgroundColor());
    m_stopwatchDisplay->setTextColor(KronometerConfig::textColor());
    m_stopwatchDisplay->showHeaders(KronometerConfig::showTimeHeaders());

    setupGranularity(KronometerConfig::showTenths(), KronometerConfig::showHundredths(), KronometerConfig::showMilliseconds());
}


void MainWindow::setupGranularity(bool tenths, bool hundredths, bool msec)
{
    if (msec) {
        m_stopwatch->setGranularity(Stopwatch::MILLISECONDS);
    }
    else if (hundredths) {
        m_stopwatch->setGranularity(Stopwatch::HUNDREDTHS);
    }
    else if (tenths) {
        m_stopwatch->setGranularity(Stopwatch::TENTHS);
    }
    else {
        m_stopwatch->setGranularity(Stopwatch::SECONDS);
    }
}

void MainWindow::slotSaveSessionAs(const QString& name)
{
    Session newSession(m_stopwatch->raw());
    newSession.setName(name);

    for (int i = 0; i < m_lapModel->rowCount(QModelIndex()); i++) {
        newSession.addLap(m_lapModel->at(i));
    }

    m_sessionModel->append(newSession);

    m_session = newSession;

    setWindowTitle(m_session.name());
    setWindowModified(false);
}

void MainWindow::loadSession()
{
    m_stopwatch->initialize(m_session.time());

    foreach (const Lap& lap, m_session.laps()) {
        m_lapModel->append(lap);
    }

    slotPaused();   // enter in paused state
    setWindowTitle(m_session.name());
}

void MainWindow::exportLapsAs(const QString& name, const QString& nameFilter)
{
    if (name.isEmpty()) {
        return;
    }

    auto exportName = name;

    if (nameFilter.contains(QLatin1String(".json"))) {
        if (not exportName.endsWith(QLatin1String(".json"))) {
            exportName += QLatin1String(".json");
        }

        QSaveFile exportFile(exportName);
        exportFile.open(QIODevice::WriteOnly);

        QJsonObject json;
        exportLapsAsJson(json);

        QJsonDocument exportDoc(json);
        exportFile.write(exportDoc.toJson());
        exportFile.commit();
    }
    else if (nameFilter.contains(QLatin1String(".csv"))) {
        if (not exportName.endsWith(QLatin1String(".csv"))) {
            exportName += QLatin1String(".csv");
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

    for (int i = 0; i < m_lapModel->rowCount(QModelIndex()); i++) {
        QJsonObject object;
        m_lapModel->at(i).write(object);
        laps.append(object);
    }

    json[QStringLiteral("laps")] = laps;
}

void MainWindow::exportLapsAsCsv(QTextStream& out)
{
    out << '#' << timestampMessage() << '\r' << '\n';
    out << '#' << i18n("Lap number,Lap time,Global time,Note") << '\r' << '\n';

    for (int i = 0; i < m_lapModel->rowCount(QModelIndex()); i++) {
        out << i;
        out << ',' << m_lapModel->at(i).relativeTime();
        out << ',' << m_lapModel->at(i).absoluteTime();
        out << ',' << m_lapModel->at(i).note();
        out << '\r' << '\n';
    }
}

QString MainWindow::timestampMessage()
{
    auto timestamp = QDateTime::currentDateTime();

    return i18n("Created by Kronometer on %1", timestamp.toString(Qt::DefaultLocaleLongDate));
}

