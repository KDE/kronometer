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
#include <KAction>

#include <QLabel>
#include <QDockWidget>
#include <QTableView>
#include <QSortFilterProxyModel>

#include "qstopwatch.h"
#include "qtimedisplay.h"
#include "lapmodel.h"
#include "widgets/generalsettings.h"
#include "widgets/fontsettings.h"
#include "widgets/savesettings.h"

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
	
    void running();
    void paused();
    void inactive();
    void showSettings();
    void writeSettings(const QString& dialogName);
	void updateLapDock();

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
	
private:
	
	QStopwatch *stopwatch;
    QTimeDisplay *stopwatchDisplay;
	QDockWidget *lapDock;
	QTableView *lapView;
	
	KAction *startAction;
	KAction *pauseAction;
	KAction *resetAction;
	KAction *lapAction;
	
	QLabel *statusLabel;
	QLabel *formatLabel;
	
	LapModel *lapModel;
    QSortFilterProxyModel *proxyModel;

    QString fileName;
    bool unsavedTimes;          /** Wheter there are unsaved times */
		
    void setupDock();
    void setupStatusBar();
	void setupActions();
	void loadSettings();

    void setupGranularity(bool tenths, bool hundredths, bool msec);

    void saveFileAs(const QString& name);
    void openFile(const QString& name);
	
};

 
#endif 
