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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUrl>

#include <KXmlGuiWindow>

class QAction;
class KStatusBar;
class QLabel;
class QSplitter;
class QTableView;
class QSortFilterProxyModel;

class Stopwatch;
class TimeDisplay;
class QTextStream;
class LapModel;

/**
 * @brief Kronometer main window.
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr, const QUrl& url = QUrl());

protected:

    bool queryClose();

private slots:

    /**
     * Stopwatch running state triggers.
     */
    void running();

    /**
     * Stopwatch paused state triggers.
     */
    void paused();

    /**
     * Stopwatch inactive state triggers.
     */
    void inactive();

    /**
     * Setup the settings dialog.
     */
    void showSettings();

    /**
     * Write the new settings on filesystem.
     * @param dialogName Ignored argument.
     */
    void writeSettings(const QString& dialogName);

    /**
     * Fix lap dock appereance.
     */
    void updateLapDock();

    /**
     * Open a new MainWindow instance.
     */
    void newFile();

    /**
     * Open an existing file in a new MainWindow instance.
     */
    void openFile();

    /**
     * Save current times on the current file.
     * @returns true if operation was successful
     */
    bool saveFile();

    /**
     * Save current times on a new file.
     * @returns true if operation was successful
     */
    bool saveFileAs();

    /**
     * Export current lap times on a file.
     */
    void exportLapsAs();

    /**
     * Copy current stopwatch time to clipboard.
     */
    void copyToClipboard();

private:

    Stopwatch *stopwatch;
    TimeDisplay *stopwatchDisplay;
    QSplitter *centralSplitter;
    QTableView *lapView;
    QLabel *statusLabel;

    QAction *startAction;
    QAction *pauseAction;
    QAction *resetAction;
    QAction *lapAction;
    QAction *exportAction;

    LapModel *lapModel;
    QSortFilterProxyModel *proxyModel;

    QUrl saveUrl;               /** Save file URL */
    bool unsavedTimes;          /** Whether there are unsaved times */

    /**
     * Setup the central widget of the window.
     */
    void setupCentralWidget();

    /**
     * Setup application status bar.
     */
    void setupStatusBar();

    /**
     * Setup standard and custom QActions.
     */
    void setupActions();

    /**
     * Load settings from app Config and apply them to the other objects.
     */
    void loadSettings();

    /**
     * Set the stopwatch refresh granularity.
     * By default, if all the arguments are false, the stopwatch is refreshed every second.
     * @param tenths Whether to refresh the stopwatch every tenth of second.
     * @param hundredths Whether to refresh the stopwatch every hundredth of second.
     * @param msec Whether to refresh the stopwatch every millisecond.
     */
    void setupGranularity(bool tenths, bool hundredths, bool msec);

    /**
     * Create a file with the current stopwatch time and lap times.
     * @param name The name of the file to be saved.
     * @return true if operation was successful
     */
    bool saveFileAs(const QString& name);

    /**
     * Load the XML save file from the member QUrl. If an error occurs, the window is closed.
     */
    void openUrl();

    /**
     * Write the XML save file on the given stream.
     * @param out The stream to be written.
     */
    void createXmlSaveFile(QTextStream& out);

    /**
     * Parse the XML save file from the given DOM document.
     * @param doc The DOM document to be parsed.
     * @return true if doc is a valid Kronometer save file, false otherwise.
     */
    bool parseXmlSaveFile(const QDomDocument& doc);

    /**
     * Export current lap times on a new file.
     * @param name The name of the file to be created.
     * @param nameFilter The name filter of the file to be created.
     */
    void exportLapsAs(const QString& name, const QString& nameFilter);

    /**
     * Write the XML laps representation on the given stream.
     * @param out The stream to be written.
     */
    void exportLapsAsXml(QTextStream& out);

    /**
     * Write the CSV laps representation on the given stream.
     * @param out The stream to be written.
     */
    void exportLapsAsCsv(QTextStream& out);

    /**
     * A "comment" message with timestamp, to be used in the created files.
     * @return The string "Created by Kronomer on <timestamp>"
     */
    QString timestampMessage();

};

 
#endif 
