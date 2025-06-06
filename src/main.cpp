/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sessionmodel.h"
#include "mainwindow.h"
#include "version.h"

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

int main (int argc, char **argv)
{
    QApplication app {argc, argv};
    KLocalizedString::setApplicationDomain("kronometer");

    KCrash::initialize();

    auto aboutData = KAboutData {
        QStringLiteral("kronometer"),   // componentName
        i18nc("KAboutData display name", "Kronometer"),
        QStringLiteral(KRONOMETER_VERSION_STRING),
        i18n("Kronometer is a simple stopwatch application"), // shortDescription
        KAboutLicense::GPL_V2,    // licenseType
        i18n("Copyright (C) 2014-2016 Elvis Angelaccio"),    // copyrightStatement
        {},  // otherText
        QStringLiteral("https://userbase.kde.org/Kronometer")   // homePageAddress
    };

    aboutData.addAuthor(
        i18n("Elvis Angelaccio"),
        i18n("Maintainer"),
        QStringLiteral("elvis.angelaccio@kde.org"),
        QStringLiteral("https://eang.it")
    );

    aboutData.addCredit(
        i18n("Ken Vermette"),
        i18n("Kronometer icon"),
        QStringLiteral("vermette@gmail.com")
    );

    KAboutData::setApplicationData(aboutData);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kronometer")));

    // Make sure that the local data directory is available.
    auto appdata = QFileInfo {QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)};
    if (!appdata.exists()) {
        auto dir = QDir {appdata.absolutePath()};
        dir.mkdir(appdata.fileName());
    }

    SessionModel sessionModel;
    auto window = new MainWindow {&sessionModel};
    window->show();

    return app.exec();
}
