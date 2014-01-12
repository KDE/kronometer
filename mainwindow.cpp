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

#include "mainwindow.h"
  
#include <KApplication>
#include <KLocale>
#include <KActionCollection>
#include <KStatusBar>
#include <KConfigDialog>

#include <QHeaderView>

#include "settings.h"

namespace
{
	const char *START_MSG = "&Start"; 
	const char *PAUSE_MSG = "&Pause";
	const char *RESET_MSG = "&Reset";
	const char *RESUME_MSG = "Re&sume";
	const char *LAP_MSG = "&Lap";
	
	const QString START_KEY = "start";
	const QString PAUSE_KEY = "pause";
	const QString RESET_KEY = "reset";
	const QString LAP_KEY = "lap";
	
	const char *RUNNING_MSG = "Running...";
	const char *PAUSED_MSG = "Paused";
	const char *INACTIVE_MSG = "Inactive"; 

}

 
MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
	statusLabel = new QLabel(this);
	formatLabel = new QLabel(this);
	stopwatch = new QStopwatch(this);

	formatLabel->setToolTip(i18n("Current time format"));
	statusLabel->setToolTip(i18n("Current chronometer status"));
	
	lapModel = new LapModel(this, "hh:mm:ss.zzz");
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(lapModel);
	
	QDockWidget *lapDock = new QDockWidget(this);
	lapDock->setObjectName("lapDock");
// 	lapDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	lapDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	lapDock->setAllowedAreas(Qt::AllDockWidgetAreas);
	lapDock->setTitleBarWidget(new QWidget(this));  // fake widget to disable titlebar
	
	lapView = new QTableView(this);
	lapView->setModel(proxyModel);
	lapView->setSelectionBehavior(QAbstractItemView::SelectRows);
	lapView->setGridStyle(Qt::DotLine);
	lapView->verticalHeader()->hide();
	lapView->resizeColumnsToContents();
	lapView->horizontalHeader()->setStretchLastSection(true);
	lapView->setSortingEnabled(true);
	lapView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
	
	lapDock->setWidget(lapView);
	addDockWidget(Qt::RightDockWidgetArea, lapDock);
		
	statusBar()->addWidget(statusLabel);
	statusBar()->addPermanentWidget(formatLabel);
	
	connect(stopwatch, SIGNAL(timeFormatChanged(QString)), this, SLOT(updatateFormatLabel(QString)));
	
	setupActions();
	loadSettings();
	
	setCentralWidget(stopwatch); 
}
 
void MainWindow::setupActions() 
{
	startAction = new KAction(this);
	pauseAction = new KAction(this);
	resetAction = new KAction(this);
	lapAction = new KAction(this);

	startAction->setIcon(KIcon("player-time"));
	startAction->setShortcut(Qt::Key_Space);
	
	pauseAction->setText(i18n(PAUSE_MSG));  // pauseAction/resetAction have fixed text (startAction don't)
	pauseAction->setIcon(KIcon("media-playback-pause"));
	pauseAction->setShortcut(Qt::Key_Space);
	
	resetAction->setText(i18n(RESET_MSG));
	resetAction->setIcon(KIcon("edit-clear-history"));
	resetAction->setShortcut(Qt::Key_F5);
	
	lapAction->setText(i18n(LAP_MSG));
	lapAction->setIcon(KIcon("chronometer"));
	lapAction->setShortcut(Qt::Key_Return);
	
	actionCollection()->addAction(START_KEY, startAction);
	actionCollection()->addAction(PAUSE_KEY, pauseAction);
	actionCollection()->addAction(RESET_KEY, resetAction);
	actionCollection()->addAction(LAP_KEY, lapAction);

	// triggers for QStopwatch "behavioral" slots
	connect(startAction, SIGNAL(triggered(bool)), stopwatch, SLOT(start()));
	connect(pauseAction, SIGNAL(triggered(bool)), stopwatch, SLOT(pause()));
	connect(resetAction, SIGNAL(triggered(bool)), stopwatch, SLOT(reset()));
	connect(lapAction, SIGNAL(triggered(bool)), stopwatch, SLOT(lap()));
	
	// triggers for LapModel slots
	connect(resetAction, SIGNAL(triggered(bool)), lapModel, SLOT(clear()));
	connect(stopwatch, SIGNAL(lap(QTime)), lapModel, SLOT(lap(QTime)));
	
	// triggers for MainWindow "gui" slots
	connect(startAction, SIGNAL(triggered(bool)), this, SLOT(running()));
	connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(paused()));
	connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(inactive()));
	connect(lapAction, SIGNAL(triggered(bool)), this, SLOT(updateLapDock()));

	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());  

	KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());
	
	setupGUI(Default, "kronometerui.rc");
	
	inactive();	// init kactions
}


void MainWindow::loadSettings()
{
	stopwatch->setTimeFormat(
		KronometerConfig::showHours(), 
		KronometerConfig::showMinutes(), 
		KronometerConfig::showSeconds(), 
		KronometerConfig::showTenths(), 
		KronometerConfig::showHundredths(), 
		KronometerConfig::showMilliseconds()
	);

}

void MainWindow::running()
{
	startAction->setEnabled(false);
	pauseAction->setEnabled(true);
	lapAction->setEnabled(true);
	statusLabel->setText(i18n(RUNNING_MSG));
}

void MainWindow::paused()
{
	startAction->setText(i18n(RESUME_MSG));
	startAction->setEnabled(true);
	pauseAction->setEnabled(false);
	lapAction->setEnabled(false);
	statusLabel->setText(i18n(PAUSED_MSG));
}

void MainWindow::inactive()
{
	startAction->setText(i18n(START_MSG));
	startAction->setEnabled(true);
	pauseAction->setEnabled(false);
	lapAction->setEnabled(false);
	statusLabel->setText(i18n(INACTIVE_MSG));
}

void MainWindow::showSettings()
{
	if (KConfigDialog::showDialog("settings"))
		return;
	
	KConfigDialog* dialog = new KConfigDialog(this, "settings", KronometerConfig::self());
	
	dialog->showButtonSeparator(true);
	
	KPageWidgetItem *generalPage = dialog->addPage(new GeneralSettings(this), i18n("General settings"));
	generalPage->setIcon(KIcon(KApplication::windowIcon()));

	connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(writeSettings(QString)));
	
	dialog->show();
}



void MainWindow::updateLapDock()
{
	lapView->resizeColumnsToContents();
	lapView->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::updatateFormatLabel(const QString& formatMsg)
{
	formatLabel->setText(formatMsg);
}


void MainWindow::writeSettings(const QString& dialogName)
{
	Q_UNUSED(dialogName);
	KronometerConfig::self()->writeConfig();
	
	loadSettings();
}



