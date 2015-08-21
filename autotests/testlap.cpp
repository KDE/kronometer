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
