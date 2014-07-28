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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs> 
#include <KIcon>
 
#include "mainwindow.h"
 
namespace
{
    const QByteArray APP_NAME = "kronometer";
    const QByteArray VERSION = "1.4.2";
    const QByteArray OTHER_TEXT = "";
    const QByteArray HOME_PAGE_ADDRESS = "http://aelog.org/kronometer";

    const QByteArray AUTHOR_EMAIL_ADDRESS = "elvis.angelaccio@kdemail.net";
    const QByteArray AUTHOR_WEB_ADDRESS = "http://aelog.org";
}

int main (int argc, char **argv)
{
    KAboutData aboutData(
        APP_NAME, APP_NAME,
        ki18n("Kronometer"),
        VERSION,
        ki18n("Kronometer is a simple chronometer application built for KDE"),
        KAboutData::License_GPL,
        ki18n("Copyright (C) 2014 Elvis Angelaccio"),
        ki18n(OTHER_TEXT),
        HOME_PAGE_ADDRESS
    );

    aboutData.addAuthor(ki18n("Elvis Angelaccio"), ki18n("Developer"), AUTHOR_EMAIL_ADDRESS, AUTHOR_WEB_ADDRESS);

    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;

    MainWindow* window = new MainWindow();
    window->setWindowIcon(KIcon("kronometer"));
    window->show();

    return app.exec();
}
