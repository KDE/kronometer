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

namespace
{
	const char *START_MSG = "&Start"; 
	const char *PAUSE_MSG = "&Pause";
	const char *RESET_MSG = "&Reset";
	const char *RESUME_MSG = "Re&sume";
	
	const QString START_KEY = "start";
	const QString PAUSE_KEY = "pause";
	const QString RESET_KEY = "reset";
	
	const char *RUNNING_MSG = "Running...";
	const char *PAUSED_MSG = "Paused";
	const char *INACTIVE_MSG = "Inactive"; 

}

 
MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
	statusLabel = new QLabel(this);
	formatLabel = new QLabel(this);
	stopwatch = new QStopwatch(this);

	formatLabel->setText(stopwatch->format());
	formatLabel->setToolTip(i18n("Current time format"));
	statusLabel->setToolTip(i18n("Current chronometer status"));
	
	setCentralWidget(stopwatch); 
	statusBar()->addWidget(statusLabel);
	statusBar()->addPermanentWidget(formatLabel);
	setupActions();
}
 
void MainWindow::setupActions() 
{
	startAction = new KAction(this);
	pauseAction = new KAction(this);
	resetAction = new KAction(this);

	startAction->setIcon(KIcon("player-time"));
	startAction->setShortcut(Qt::Key_Space);
	
	pauseAction->setText(i18n(PAUSE_MSG));  // pauseAction/resetAction have fixed text (startAction don't)
	pauseAction->setIcon(KIcon("media-playback-pause"));
	pauseAction->setShortcut(Qt::Key_Space);
	
	resetAction->setText(i18n(RESET_MSG));
	resetAction->setIcon(KIcon("edit-clear-history"));
	resetAction->setShortcut(Qt::Key_F5);
	
	actionCollection()->addAction(START_KEY, startAction);
	actionCollection()->addAction(PAUSE_KEY, pauseAction);
	actionCollection()->addAction(RESET_KEY, resetAction);

	// triggers for Chronometer "behavioral" slots
	connect(startAction, SIGNAL(triggered(bool)), stopwatch, SLOT(start()));
	connect(pauseAction, SIGNAL(triggered(bool)), stopwatch, SLOT(pause()));
	connect(resetAction, SIGNAL(triggered(bool)), stopwatch, SLOT(reset()));
	
	// triggers for MainWindow "gui" slots
	connect(startAction, SIGNAL(triggered(bool)), this, SLOT(running()));
	connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(paused()));
	connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(inactive()));

	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());  

	setupGUI(Default, "kronometerui.rc");
	
	inactive();	// init kactions
}



void MainWindow::running()
{
	startAction->setEnabled(false);
	pauseAction->setEnabled(true);
	statusLabel->setText(i18n(RUNNING_MSG));
}

void MainWindow::paused()
{
	startAction->setText(i18n(RESUME_MSG));
	startAction->setEnabled(true);
	pauseAction->setEnabled(false);
	statusLabel->setText(i18n(PAUSED_MSG));
}

void MainWindow::inactive()
{
	startAction->setText(i18n(START_MSG));
	startAction->setEnabled(true);
	pauseAction->setEnabled(false);
	statusLabel->setText(i18n(INACTIVE_MSG));
}




