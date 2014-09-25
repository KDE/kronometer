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

#include <QApplication>
#include <KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"

int main (int argc, char **argv)
{
    KAboutData aboutData(
        QStringLiteral("kronometer"),   // componentName
        i18n("Kronometer"), // displayName
        QStringLiteral("1.5.2"),    // version
        i18n("Kronometer is a simple stopwatch application built for KDE"), // shortDescription
        KAboutLicense::GPL_V2,    // licenseType
        i18n("Copyright (C) 2014 Elvis Angelaccio"),    // copyrightStatement
        QString(),  // otherText
        QStringLiteral("http://aelog.org/kronometer")   // homePageAddress
    );

    aboutData.addAuthor(
        i18n("Elvis Angelaccio"),
        i18n("Developer"),
        QStringLiteral("elvis.angelaccio@kdemail.net"),
        QStringLiteral("http://aelog.org")
    );

    KAboutData::setApplicationData(aboutData);

    QApplication app(argc, argv);
    app.setApplicationName(aboutData.componentName());
    app.setApplicationDisplayName(aboutData.displayName());
    app.setOrganizationDomain(aboutData.organizationDomain());
    app.setApplicationVersion(aboutData.version());
    app.setWindowIcon(QIcon::fromTheme("kronometer"));

    MainWindow* window = new MainWindow();
    window->show();

    return app.exec();
}
