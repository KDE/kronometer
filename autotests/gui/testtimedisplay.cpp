/*
    Copyright (C) 2016 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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
