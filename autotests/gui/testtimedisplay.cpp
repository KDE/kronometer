/*
    SPDX-FileCopyrightText: 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testtimedisplay.h"
#include "timedisplay.h"

#include "digitdisplay.h"

#include <QGroupBox>

#include <KColorScheme>

void TestTimeDisplay::testDefaultWidget()
{
    TimeDisplay timeDisplay;

    const auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    QCOMPARE(groupBoxes.size(), 4);

    for (auto group : groupBoxes) {
        QCOMPARE(group->findChildren<DigitDisplay*>().size(), 1);
        QVERIFY(!group->title().isEmpty());
    }
}

void TestTimeDisplay::testSetBackgroundColor()
{
    KColorScheme scheme {QPalette::Active};
    const auto color = scheme.foreground(KColorScheme::NegativeText).color();

    TimeDisplay timeDisplay;
    timeDisplay.setBackgroundColor(color);

    const auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    for (auto group : groupBoxes) {
        QCOMPARE(group->palette().color(group->backgroundRole()), color);
    }
}

void TestTimeDisplay::testSetTextColor()
{
    KColorScheme scheme {QPalette::Active};
    const auto color = scheme.foreground(KColorScheme::NegativeText).color();

    TimeDisplay timeDisplay;
    timeDisplay.setTextColor(color);

    const auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    for (auto group : groupBoxes) {
        QCOMPARE(group->palette().color(group->foregroundRole()), color);
    }
}

QTEST_MAIN(TestTimeDisplay)
