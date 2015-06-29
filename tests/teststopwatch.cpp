/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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

#include "stopwatch.h"
#include "teststopwatch.h"

#include <QTime>

void TestStopwatch::testInactive()
{
    Stopwatch stopwatch;

    QVERIFY(not stopwatch.isRunning());
    QVERIFY(not stopwatch.isPaused());
    QVERIFY(stopwatch.isInactive());
}

void TestStopwatch::testRunning()
{
    Stopwatch stopwatch;

    stopwatch.slotStart();

    QVERIFY(stopwatch.isRunning());
    QVERIFY(not stopwatch.isPaused());
    QVERIFY(not stopwatch.isInactive());
}

void TestStopwatch::testPaused()
{
    Stopwatch stopwatch;

    stopwatch.slotPause();

    QVERIFY(not stopwatch.isRunning());
    QVERIFY(stopwatch.isPaused());
    QVERIFY(not stopwatch.isInactive());
}

void TestStopwatch::testReset()
{
    Stopwatch stopwatch;

    stopwatch.slotReset();

    QVERIFY(not stopwatch.isRunning());
    QVERIFY(not stopwatch.isPaused());
    QVERIFY(stopwatch.isInactive());
}

void TestStopwatch::testInitialize()
{
    Stopwatch s1, s2;

    s1.slotStart();
    QTest::qSleep(100);
    s1.slotPause();
    qint64 t = s1.raw();

    QVERIFY(s2.initialize(t));
    QVERIFY(s2.isPaused());
    QCOMPARE(s2.raw(), t);
}

QTEST_MAIN(TestStopwatch)
