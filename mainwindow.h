/*
	Copyright (C) 20014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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
#include "lapmodel.h"

#include "widgets/generalsettings.h"

class MainWindow : public KXmlGuiWindow
{
	Q_OBJECT

public:
	
	MainWindow(QWidget *parent=0);

public slots:
	
	void running();
	void paused();
	void inactive();
	
	void showSettings();
	
private slots:
	
	void updateLapDock();
	void updatateFormatLabel(const QString& formatMsg);
	void writeSettings(const QString& dialogName);
	
private:
	
	QStopwatch *stopwatch;
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
		
	void setupActions();
	void loadSettings();
	
};

 
#endif 
