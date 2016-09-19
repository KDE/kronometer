/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

int main (int argc, char **argv)
{
    KLocalizedString::setApplicationDomain("kronometer");

    QApplication app {argc, argv};

    KCrash::initialize();

    auto aboutData = KAboutData {
        QStringLiteral("kronometer"),   // componentName
        i18nc("KAboutData display name", "Kronometer"),
        QStringLiteral("2.1.1"),    // version
        i18n("Kronometer is a simple stopwatch application"), // shortDescription
        KAboutLicense::GPL_V2,    // licenseType
        i18n("Copyright (C) 2014-2016 Elvis Angelaccio"),    // copyrightStatement
        {},  // otherText
        QStringLiteral("http://aelog.org/kronometer")   // homePageAddress
    };

    aboutData.addAuthor(
        i18n("Elvis Angelaccio"),
        i18n("Maintainer"),
        QStringLiteral("elvis.angelaccio@kde.org"),
        QStringLiteral("http://aelog.org")
    );

    aboutData.addCredit(
        i18n("Ken Vermette"),
        i18n("Kronometer icon"),
        QStringLiteral("vermette@gmail.com")
    );

    KAboutData::setApplicationData(aboutData);

    app.setApplicationName(aboutData.componentName());
    app.setApplicationDisplayName(aboutData.displayName());
    app.setOrganizationDomain(aboutData.organizationDomain());
    app.setApplicationVersion(aboutData.version());
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kronometer")));

    // Make sure that the local data directory is available.
    auto appdata = QFileInfo {QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)};
    if (not appdata.exists()) {
        auto dir = QDir {appdata.absolutePath()};
        dir.mkdir(appdata.fileName());
    }

    auto window = new MainWindow {};
    window->show();

    return app.exec();
}
