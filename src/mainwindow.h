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

#include "session.h"

#include <KXmlGuiWindow>

class LapModel;
class SessionModel;
class Stopwatch;
class TimeDisplay;

class QAction;
class QLabel;
class QSortFilterProxyModel;
class QSplitter;
class QTableView;
class QTextStream;

/**
 * @brief Kronometer main window.
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr, const Session& session = Session());

public slots:

    void setWindowTitle(const QString& title);

protected:

    bool queryClose();

private slots:

    /**
     * Stopwatch running state triggers.
     */
    void slotRunning();

    /**
     * Stopwatch paused state triggers.
     */
    void slotPaused();

    /**
     * Stopwatch inactive state triggers.
     */
    void slotInactive();

    /**
     * Setup the settings dialog.
     */
    void slotShowSettings();

    /**
     * Write the new settings on filesystem.
     * @param dialogName Ignored argument.
     */
    void slotWriteSettings(const QString& dialogName);

    /**
     * Fix lap dock appereance.
     */
    void slotUpdateLapDock();

    /**
     * Open a new MainWindow instance.
     */
    void slotNewSession();

    /**
     * Open an existing session in a new MainWindow instance.
     */
    void slotOpenSession();

    /**
     * Save current times in the current session.
     */
    void slotSaveSession();

    /**
     * Save current times as a new session.
     */
    void slotSaveSessionAs();

    /**
     * Export current lap times on a file.
     */
    void slotExportLapsAs();

    /**
     * Copy current stopwatch time to clipboard.
     */
    void slotCopyToClipboard();

private:

    Stopwatch *m_stopwatch;
    TimeDisplay *m_stopwatchDisplay;
    QSplitter *m_centralSplitter;
    QTableView *m_lapView;
    QLabel *m_statusLabel;

    QAction *m_startAction;
    QAction *m_pauseAction;
    QAction *m_resetAction;
    QAction *m_lapAction;
    QAction *m_exportAction;

    LapModel *m_lapModel;
    SessionModel *m_sessionModel;

    Session m_session;

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
     * Create a session with the current stopwatch time and lap times.
     * @param name The name of the session to be saved.
     */
    void slotSaveSessionAs(const QString& name);

    /**
     * Load a saved session.
     */
    void loadSession();

    /**
     * Export current lap times on a new file.
     * @param name The name of the file to be created.
     * @param nameFilter The name filter of the file to be created.
     */
    void exportLapsAs(const QString& name, const QString& nameFilter);

    /**
     * Write the JSON laps representation on the given object.
     * @param json The JSON object to be written.
     */
    void exportLapsAsJson(QJsonObject& json);

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
