/*
    Copyright (C) 2016 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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

void TestTimeDisplay::testDefaultWidget()
{
    TimeDisplay timeDisplay;

    auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    QCOMPARE(groupBoxes.size(), 4);

    foreach (auto group, groupBoxes) {
        QCOMPARE(group->findChildren<DigitDisplay*>().size(), 1);
        QVERIFY(!group->title().isEmpty());
    }
}

void TestTimeDisplay::testSetBackgroundColor()
{
    QColor color {Qt::red};

    TimeDisplay timeDisplay;
    timeDisplay.setBackgroundColor(color);

    auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    foreach (auto group, groupBoxes) {
        QCOMPARE(group->palette().color(group->backgroundRole()), color);
    }
}

void TestTimeDisplay::testSetTextColor()
{
    QColor color {Qt::red};

    TimeDisplay timeDisplay;
    timeDisplay.setTextColor(color);

    auto groupBoxes = timeDisplay.findChildren<QGroupBox*>();
    foreach (auto group, groupBoxes) {
        QCOMPARE(group->palette().color(group->foregroundRole()), color);
    }
}

QTEST_MAIN(TestTimeDisplay)
