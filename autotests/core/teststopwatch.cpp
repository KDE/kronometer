/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teststopwatch.h"
#include "stopwatch.h"

#include <QTime>

void TestStopwatch::testInactive()
{
    Stopwatch stopwatch;

    QVERIFY(!stopwatch.isRunning());
    QVERIFY(!stopwatch.isPaused());
    QVERIFY(stopwatch.isInactive());
}

void TestStopwatch::testRunning()
{
    Stopwatch stopwatch;

    stopwatch.start();

    QVERIFY(stopwatch.isRunning());
    QVERIFY(!stopwatch.isPaused());
    QVERIFY(!stopwatch.isInactive());
}

void TestStopwatch::testPaused()
{
    Stopwatch stopwatch;

    stopwatch.pause();

    QVERIFY(!stopwatch.isRunning());
    QVERIFY(stopwatch.isPaused());
    QVERIFY(!stopwatch.isInactive());
}

void TestStopwatch::testReset()
{
    Stopwatch stopwatch;

    stopwatch.reset();

    QVERIFY(!stopwatch.isRunning());
    QVERIFY(!stopwatch.isPaused());
    QVERIFY(stopwatch.isInactive());
}

void TestStopwatch::testInitialize()
{
    Stopwatch s1, s2;

    s1.start();
    QTest::qSleep(100);
    s1.pause();
    int t = s1.raw();

    QVERIFY(s2.initialize(t));
    QVERIFY(s2.isPaused());
    QCOMPARE(s2.raw(), t);
}

QTEST_MAIN(TestStopwatch)
