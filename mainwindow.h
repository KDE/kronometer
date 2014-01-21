/*
    Copyright (C) 2014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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
 
#include <KXmlGuiWindow>

class KAction;
class QLabel;
class QDockWidget;
class QTableView;
class QSortFilterProxyModel;

class QStopwatch;
class QTimeDisplay;
class LapModel;

/**
 * @brief Kronometer main window.
 */
class MainWindow : public KXmlGuiWindow
{
	Q_OBJECT

public:
	
    explicit MainWindow(QWidget *parent = nullptr, const QString& file = "");

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
     */
    void saveFile();

    /**
     * Save current times on a new file.
     */
    void saveFileAs();
	
private:
	
	QStopwatch *stopwatch;
    QTimeDisplay *stopwatchDisplay;
	QDockWidget *lapDock;
	QTableView *lapView;
	QLabel *statusLabel;
	
	KAction *startAction;
	KAction *pauseAction;
	KAction *resetAction;
	KAction *lapAction;
	
	LapModel *lapModel;
    QSortFilterProxyModel *proxyModel;

    QString fileName;
    bool unsavedTimes;          /** Wheter there are unsaved times */
		
    /**
     * Setup application dock widgets.
     */
    void setupDock();

    /**
     * Setup application status bar.
     */
    void setupStatusBar();

    /**
     * Setup standard and custom KActions.
     */
	void setupActions();

    /**
     * Load settings from app Config and apply them to the other objects.
     */
	void loadSettings();

    /**
     * Set the stopwatch refresh granularity.
     * By default, if all the arguments are false, the stopwatch is refreshed every second.
     * @param tenths Wheter to refresh the stopwatch every tenth of second.
     * @param hundredths Wheter to refresh the stopwatch every hundredth of second.
     * @param msec Wheter to refresh the stopwatch every millisecond.
     */
    void setupGranularity(bool tenths, bool hundredths, bool msec);

    /**
     * Crate a file with the current stopwatch time and lap times.
     * @param name The name of the file to be saved.
     */
    void saveFileAs(const QString& name);

    /**
     * Load from file the stopwatch and lap times.
     * @param name The name of the tile with the saved times to be loaded.
     */
    void openFile(const QString& name);
};

 
#endif 
