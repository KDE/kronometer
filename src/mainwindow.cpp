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

#include "mainwindow.h"

#include <KLocalizedString>
#include <KActionCollection>
#include <KConfigDialog>
#include <KMessageBox>
#include <KJobUiDelegate>
#include <KIO/FileCopyJob>

#include <QApplication>
#include <QAction>
#include <QStatusBar>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QSaveFile>
#include <QTableView>
#include <QSplitter>
#include <QClipboard>
#include <QSortFilterProxyModel>
#include <QDomDocument>
#include <QPointer>

#include "stopwatch.h"
#include "timedisplay.h"
#include "lapmodel.h"
#include "settings.h"
#include "widgets/generalsettings.h"
#include "widgets/fontsettings.h"
#include "widgets/colorsettings.h"
#include "widgets/savesettings.h"
#include "widgets/guisettings.h"

namespace
{
    const QString START_KEY = "start";
    const QString PAUSE_KEY = "pause";
    const QString RESET_KEY = "reset";
    const QString LAP_KEY = "lap";
    const QString EXPORT_KEY = "export_laps";

    const QString WINDOW_TITLE = "Kronometer";       /** Default Window title */
    const QString QT_PLACE_HOLDER = "[*]";           /** Qt standard placeholder for setWindowModified() */

    const QString XML_MIMETYPE = "application/xml";
    const QString CSV_MIMETYPE = "text/csv";
    const QString XML_EXTENSION = ".xml";
    const QString CSV_EXTENSION = ".csv";

    // kronometerui.rc states
    const QString INACTIVE_STATE = "inactive";
    const QString RUNNING_STATE = "running";
    const QString PAUSED_STATE = "paused";
    const QString PAUSED_FILE_STATE = "pausedFile";  /** An open file has been paused */

    // XML strings
    const QString STOPWATCH_TAG = "stopwatch";
    const QString LAPS_TAG = "laps";
    const QString LAP_TAG = "lap";
    const QString TIME_TAG = "time";
    const QString NOTE_TAG = "note";
    const QString ROOT_TAG = "kronometer";
    const QString PERSISTENCE_ATTR = "msec";
    const QString TYPE_ATTR = "type";
    const QString LAP_ID_ATTR = "id";
    const QString REL_TYPE = "relative";
    const QString ABS_TYPE = "absolute";
}

 
MainWindow::MainWindow(QWidget *parent, const QUrl& url) : KXmlGuiWindow(parent), saveUrl(url), unsavedTimes(false)
{
    stopwatch = new Stopwatch(this);
    stopwatchDisplay = new TimeDisplay(this);
    connect(stopwatch, SIGNAL(time(QTime)), stopwatchDisplay, SLOT(onTime(QTime)));  // bind stopwatch to its display

    setupCentralWidget();
    setupStatusBar();
    setupActions();
    loadSettings();

    setWindowTitle(WINDOW_TITLE + QT_PLACE_HOLDER);

    if (not saveUrl.isEmpty()) {
        openUrl();
    }
}

bool MainWindow::queryClose()
{
    if (stopwatch->isInactive() or not KronometerConfig::askOnExit()) {
        return true;  // exit without ask
    }

    if (stopwatch->isRunning()) {
        stopwatch->onPause();
        paused();
    }

    int buttonCode;

    if (saveUrl.isEmpty()) {
        buttonCode = KMessageBox::warningYesNoCancel(this, i18n("Save times on a new file?"));

        switch (buttonCode) {
        case KMessageBox::Yes:
            return saveFileAs();
        case KMessageBox::No:
            return true;
        default: // cancel
            return false;
        }
    }
    else if (unsavedTimes) {
        buttonCode = KMessageBox::warningYesNoCancel(this, i18n("Save times to file %1?", saveUrl.fileName()));

        switch (buttonCode) {
        case KMessageBox::Yes:
            // save document here. If saving fails, return false;
            return saveFile();
        case KMessageBox::No:
            return true;
        default: // cancel
            return false;
        }
    }

    return true;  // there is an open file, but times are already saved.
}
 
void MainWindow::running()
{
    statusLabel->setText(i18n("Running..."));

    unsavedTimes = true;
    setWindowModified(unsavedTimes);

    stateChanged(RUNNING_STATE);
}

void MainWindow::paused()
{
    startAction->setText(i18n("Re&sume"));
    statusLabel->setText(i18n("Paused"));

    if (not saveUrl.isEmpty()) {
        stateChanged(PAUSED_FILE_STATE);
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

void MainWindow::inactive()
{
    startAction->setText(i18n("&Start"));
    statusLabel->setText(i18n("Inactive"));

    unsavedTimes = false;
    setWindowModified(unsavedTimes);

    stateChanged(INACTIVE_STATE);
}

void MainWindow::showSettings()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(this, "settings", KronometerConfig::self());

    // dialog->showButtonSeparator(true); // TODO: figure out if and how to port to KF5

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

    connect(dialog, SIGNAL(settingsChanged(QString)), this, SLOT(writeSettings(QString)));

    dialog->show();
}

void MainWindow::writeSettings(const QString& dialogName)
{
    Q_UNUSED(dialogName);
    KronometerConfig::self()->save();

    MainWindow *window = nullptr;

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        window = qobject_cast<MainWindow *>(widget);

        if (window) {
            window->loadSettings();
        }
    }
}

void MainWindow::updateLapDock()
{
    lapView->resizeColumnsToContents();
    lapView->horizontalHeader()->setStretchLastSection(true);
    lapView->selectRow(lapModel->rowCount(QModelIndex()) - 1);  // rows indexes start from 0
}

void MainWindow::newFile()
{
    MainWindow *window = new MainWindow();
    window->show();
}

void MainWindow::openFile()
{
    QPointer<QFileDialog> dialog = new QFileDialog(this);
    dialog->setWindowTitle(i18n("Choose a Kronometer save file"));

    QStringList mimeTypes;
    mimeTypes << XML_MIMETYPE;
    dialog->setMimeTypeFilters(mimeTypes);

    if (dialog->exec() == QDialog::Accepted) {
        QUrl file = QUrl::fromUserInput(dialog->selectedFiles().first());

        if (not file.isEmpty()) {
            MainWindow *window = new MainWindow(nullptr, file);
            window->show();
        }
    }

    delete dialog;
}

bool MainWindow::saveFile()
{
    return saveFileAs(saveUrl.fileName());
}

bool MainWindow::saveFileAs()
{
    QPointer<QFileDialog> dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setConfirmOverwrite(true);
    dialog->setWindowTitle(i18n("Choose Kronometer save file destination"));

    QStringList mimeTypes;
    mimeTypes << XML_MIMETYPE;
    dialog->setMimeTypeFilters(mimeTypes);

    bool rc = false;
    if (dialog->exec() == QDialog::Accepted) {
        rc = saveFileAs(dialog->selectedFiles().first());
    }

    delete dialog;
    return rc;
}

void MainWindow::exportLapsAs()
{
    QPointer<QFileDialog> dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setConfirmOverwrite(true);
    dialog->setWindowTitle(i18n("Choose export file destination"));

    QStringList mimeTypes;
    mimeTypes << CSV_MIMETYPE << XML_MIMETYPE;
    dialog->setMimeTypeFilters(mimeTypes);

    if (dialog->exec() == QDialog::Accepted) {
        exportLapsAs(dialog->selectedFiles().first(), dialog->selectedNameFilter());
    }

    delete dialog;
}

void MainWindow::copyToClipboard()
{
    QApplication::clipboard()->setText(stopwatchDisplay->currentTime());
}

void MainWindow::setupCentralWidget()
{
    centralSplitter = new QSplitter(this);

    lapModel = new LapModel(this);
    proxyModel = new QSortFilterProxyModel(this);
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
    connect(startAction, SIGNAL(triggered(bool)), stopwatch, SLOT(onStart()));
    connect(pauseAction, SIGNAL(triggered(bool)), stopwatch, SLOT(onPause()));
    connect(resetAction, SIGNAL(triggered(bool)), stopwatch, SLOT(onReset()));
    connect(lapAction, SIGNAL(triggered(bool)), stopwatch, SLOT(onLap()));

    // triggers for LapModel slots
    connect(resetAction, SIGNAL(triggered(bool)), lapModel, SLOT(onClear()));
    connect(stopwatch, SIGNAL(lap(QTime)), lapModel, SLOT(onLap(QTime)));

    // triggers for MainWindow "gui" slots
    connect(startAction, SIGNAL(triggered(bool)), this, SLOT(running()));
    connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(paused()));
    connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(inactive()));
    connect(lapAction, SIGNAL(triggered(bool)), this, SLOT(updateLapDock()));

    // File menu triggers
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());
    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
    KStandardAction::save(this, SLOT(saveFile()), actionCollection());
    KStandardAction::saveAs(this, SLOT(saveFileAs()), actionCollection());
    KStandardAction::open(this, SLOT(openFile()), actionCollection());
    KStandardAction::copy(this, SLOT(copyToClipboard()), actionCollection());
    connect(exportAction, SIGNAL(triggered(bool)), this, SLOT(exportLapsAs()));

    setupGUI(Default, "kronometerui.rc");

    inactive();	// inactive state is the default
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

    lapAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    exportAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    lapView->setVisible(KronometerConfig::isLapsRecordingEnabled());
    lapModel->setTimeFormat(timeFormat);
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

bool MainWindow::saveFileAs(const QString& name)
{
    if (name.isEmpty()) {
        return false;
    }

    QString saveName = name;

    if (not saveName.endsWith(XML_EXTENSION)) {
        saveName += XML_EXTENSION;
    }

    QSaveFile saveFile(saveName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
         KMessageBox::error(this, i18n("Failed to open file"));
        return false;
    }

    // Persistence using XML files
    QTextStream stream(&saveFile);
    createXmlSaveFile(stream);


    bool isSaveSuccessfull = saveFile.commit();

    if (isSaveSuccessfull) {
        saveUrl = QUrl(saveName);

        unsavedTimes = false;
        setWindowModified(unsavedTimes);
        return true;
    }
    else {
        return false;
    }
}

void MainWindow::openUrl()
{
    QString targetPath;
    bool ready = false;

    if (saveUrl.isLocalFile()) {
        targetPath = saveUrl.toLocalFile();
        ready = true;
    }

    else {
        QTemporaryFile tmpFile;
        tmpFile.open();
        QUrl tmpUrl = QUrl::fromLocalFile(tmpFile.fileName());

        KIO::FileCopyJob *fileCopyJob = KIO::file_copy(saveUrl, tmpUrl, -1, KIO::Overwrite);
        ready = fileCopyJob->exec();

        if (!ready) {
            fileCopyJob->uiDelegate()->showErrorMessage();
            close();  // files are opened in a new window, so if the open fails the new window has to be closed.
        }
    }

    if (ready) {
        QFile file(targetPath);
        file.open(QIODevice::ReadOnly);

        // Persistence using XML files
        QDomDocument doc;
        QString errorMsg;

        if (doc.setContent(&file, &errorMsg)) {
            if (parseXmlSaveFile(doc)) {
                paused();                       // enter in paused state

                setWindowTitle(WINDOW_TITLE + " - " + saveUrl.fileName() + QT_PLACE_HOLDER);
            }
            else {
                close();
            }
        }
        else {
            KMessageBox::error(this, i18n("Cannot open file: %1", errorMsg));
            close();
        }
    }
}

void MainWindow::createXmlSaveFile(QTextStream& out)
{
    QDomDocument doc;
    QDomProcessingInstruction metaData = doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    QDomComment timestampComment = doc.createComment(timestampMessage());
    QDomElement rootElement = doc.createElement(ROOT_TAG);

    QDomElement stopwatchElement = doc.createElement(STOPWATCH_TAG);
    stopwatchElement.setAttribute(PERSISTENCE_ATTR, stopwatch->raw());
    QDomElement stopwatchTime = doc.createElement(TIME_TAG);
    stopwatchTime.setAttribute(TYPE_ATTR, ABS_TYPE);
    stopwatchTime.appendChild(doc.createTextNode(stopwatchDisplay->currentTime()));
    stopwatchElement.appendChild(stopwatchTime);

    QDomElement lapsElement = doc.createElement(LAPS_TAG);

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        QDomElement lap = doc.createElement(LAP_TAG);
        lap.setAttribute(LAP_ID_ATTR, i);
        lap.setAttribute(PERSISTENCE_ATTR, lapModel->at(i).raw());

        QDomElement relTime = doc.createElement(TIME_TAG);
        relTime.setAttribute(TYPE_ATTR, REL_TYPE);
        relTime.appendChild(doc.createTextNode(lapModel->at(i).relativeTime()));

        QDomElement absTime = doc.createElement(TIME_TAG);
        absTime.setAttribute(TYPE_ATTR, ABS_TYPE);
        absTime.appendChild(doc.createTextNode(lapModel->at(i).absoluteTime()));

        lap.appendChild(relTime);
        lap.appendChild(absTime);

        if (lapModel->at(i).hasNote()) {
            QDomElement lapNote = doc.createElement(NOTE_TAG);
            lapNote.appendChild(doc.createTextNode(lapModel->at(i).note()));

            lap.appendChild(lapNote);
        }

        lapsElement.appendChild(lap);
    }

    rootElement.appendChild(stopwatchElement);
    rootElement.appendChild(lapsElement);
    doc.appendChild(metaData);
    doc.appendChild(timestampComment);
    doc.appendChild(rootElement);
    doc.save(out, KronometerConfig::saveFileIndentSize());
}

bool MainWindow::parseXmlSaveFile(const QDomDocument& doc)
{
    QDomElement rootElement = doc.namedItem(ROOT_TAG).toElement();

    if (rootElement.isNull()) {
        KMessageBox::error(this, i18n("Invalid XML file"));
        return false;
    }

    QDomElement stopwatchElement = rootElement.namedItem(STOPWATCH_TAG).toElement();
    QDomElement lapsElement = rootElement.namedItem(LAPS_TAG).toElement();

    if (stopwatchElement.isNull() or lapsElement.isNull()) {
        KMessageBox::error(this, i18n("Incomplete Kronometer save file"));
        return false;
    }

    QString data = stopwatchElement.attribute(PERSISTENCE_ATTR);
    stopwatch->initialize(data.toLongLong());  // TODO: check return value

    QDomElement lap = lapsElement.firstChildElement(LAP_TAG);

    while (not lap.isNull()) {
        QString data = lap.attribute(PERSISTENCE_ATTR);
        Lap newLap = Lap::fromRawData(data.toLongLong());
        QDomElement note = lap.namedItem(NOTE_TAG).toElement();

        if (not note.isNull()) {
            newLap.setNote(note.text());
        }

        lapModel->append(newLap);
        lap = lap.nextSiblingElement(LAP_TAG);
    }

    return true;
}

void MainWindow::exportLapsAs(const QString& name, const QString& nameFilter)
{
    if (name.isEmpty()) {
        return;
    }

    QString exportName = name;

    if (nameFilter.contains(XML_EXTENSION)) {
        if (not exportName.endsWith(XML_EXTENSION)) {
            exportName += XML_EXTENSION;
        }

        QSaveFile exportFile(exportName);
        exportFile.open(QIODevice::WriteOnly);

        QTextStream stream(&exportFile);
        exportLapsAsXml(stream);

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

void MainWindow::exportLapsAsXml(QTextStream& out)
{
    QDomDocument doc;
    QDomProcessingInstruction metaData = doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    QDomComment timestampComment = doc.createComment(timestampMessage());
    QDomElement rootElement = doc.createElement(ROOT_TAG);

    QDomElement lapsElement = doc.createElement(LAPS_TAG);

    for (int i = 0; i < lapModel->rowCount(QModelIndex()); i++) {
        QDomElement lap = doc.createElement(LAP_TAG);
        lap.setAttribute(LAP_ID_ATTR, i);

        QDomElement relTime = doc.createElement(TIME_TAG);
        relTime.setAttribute(TYPE_ATTR, REL_TYPE);
        relTime.appendChild(doc.createTextNode(lapModel->at(i).relativeTime()));

        QDomElement absTime = doc.createElement(TIME_TAG);
        absTime.setAttribute(TYPE_ATTR, ABS_TYPE);
        absTime.appendChild(doc.createTextNode(lapModel->at(i).absoluteTime()));

        lap.appendChild(relTime);
        lap.appendChild(absTime);

        if (lapModel->at(i).hasNote()) {
            QDomElement lapNote = doc.createElement(NOTE_TAG);
            lapNote.appendChild(doc.createTextNode(lapModel->at(i).note()));

            lap.appendChild(lapNote);
        }

        lapsElement.appendChild(lap);
    }

    rootElement.appendChild(lapsElement);
    doc.appendChild(metaData);
    doc.appendChild(timestampComment);
    doc.appendChild(rootElement);
    doc.save(out, KronometerConfig::exportedXmlIndentSize());
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

