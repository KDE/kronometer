/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testlap.h"
#include "lap.h"

#include <QTime>

void TestLap::testDefaultLap()
{
    Lap lap;

    QCOMPARE(lap.time(), QTime(0, 0));
    QCOMPARE(lap.raw(), 0);
    QVERIFY(lap.relativeTime().isEmpty());
    QVERIFY(lap.absoluteTime().isEmpty());
}

void TestLap::testLapTime()
{
    QTime time {1, 30};
    Lap lap {time};

    QCOMPARE(lap.time(), time);
}

void TestLap::testRelativeTime()
{
    Lap lap;
    auto test = QLatin1String("test");

    lap.setRelativeTime(test);

    QCOMPARE(lap.relativeTime(), test);
}

void TestLap::testAbsoluteTime()
{
    Lap lap;
    auto test = QLatin1String("test");

    lap.setAbsoluteTime(test);

    QCOMPARE(lap.absoluteTime(), test);
}

void TestLap::testNote()
{
    Lap lap;
    auto test = QLatin1String("test");

    lap.setNote(test);

    QCOMPARE(lap.note(), test);
}

void TestLap::testRawData()
{
    int t = 1000;
    Lap lap = Lap::fromRawData(t);

    QCOMPARE(lap.raw(), t);
}

void TestLap::testTimeTo()
{
    int t1 = 50;
    int t2 = 100;

    Lap lap1 = Lap::fromRawData(t1);
    Lap lap2 = Lap::fromRawData(t2);

    QCOMPARE(lap1.timeTo(lap2).msec(), t2 - t1);
}

QTEST_MAIN(TestLap)
