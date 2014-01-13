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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs> 
 
#include "mainwindow.h"
 
namespace
{
	const QByteArray APP_NAME = "kronometer";
	const QByteArray PROGRAM_NAME = "Kronometer";
	const QByteArray VERSION = "0.4.0";
	const QByteArray SHORT_DESCRIPTION = "Kronometer is a simple chronometer application built for KDE";
	const QByteArray COPYRIGHT_STATEMENT = "Copyright (C) 2014 Elvis Angelaccio";
	const QByteArray OTHER_TEXT = "";
	const QByteArray HOME_PAGE_ADDRESS = "https://github.com/elvisangelaccio/kronometer";
	const QByteArray BUGS_EMAIL_ADDRESS = "angelaccioelvis@gmail.com";
	
	const QByteArray AUTHOR_NAME = "Elvis Angelaccio";
	const QByteArray AUTHOR_TASK = "Developer";
	const QByteArray AUTHOR_EMAIL_ADDRESS = "angelaccioelvis@gmail.com";
	const QByteArray AUTHOR_WEB_ADDRESS = "https://github.com/elvisangelaccio/kronometer";
}

int main (int argc, char **argv)
{
	KAboutData aboutData(
		APP_NAME, APP_NAME,
		ki18n(PROGRAM_NAME), 
		VERSION,
		ki18n(SHORT_DESCRIPTION),
		KAboutData::License_GPL,
		ki18n(COPYRIGHT_STATEMENT),
		ki18n(OTHER_TEXT),
		HOME_PAGE_ADDRESS,
		BUGS_EMAIL_ADDRESS
	);
	
	aboutData.addAuthor(ki18n(AUTHOR_NAME), ki18n(AUTHOR_TASK), AUTHOR_EMAIL_ADDRESS, AUTHOR_WEB_ADDRESS);
	
	KCmdLineArgs::init(argc, argv, &aboutData);
	
	KApplication app;

	MainWindow* window = new MainWindow();
	window->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	window->setWindowIcon(KIcon("kronometer"));
	window->show();
	
	return app.exec();
}
