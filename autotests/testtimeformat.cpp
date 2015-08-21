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

#include "testtimeformat.h"
#include "timeformat.h"

#include <QTime>

void TestTimeFormat::testDefaultFormat()
{
    TimeFormat timeFormat;
    QTime t {0, 0};

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00.00"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::UpToHundredths);
    QVERIFY(not timeFormat.hasHours());
    QVERIFY(timeFormat.hasMinutes());
    QVERIFY(timeFormat.hasFractions());
}

void TestTimeFormat::testFullFormat()
{
    TimeFormat timeFormat {true, true, TimeFormat::UpToMilliseconds};
    QTime t {0, 0};

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00:00.000"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::UpToMilliseconds);
    QVERIFY(timeFormat.hasHours());
    QVERIFY(timeFormat.hasMinutes());
    QVERIFY(timeFormat.hasFractions());
}

void TestTimeFormat::testMinimalFormat()
{
    TimeFormat timeFormat {false, false, TimeFormat::NoFractions};
    QTime t {0, 0};

    QCOMPARE(timeFormat.format(t), QStringLiteral("00"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::NoFractions);
    QVERIFY(not timeFormat.hasHours());
    QVERIFY(not timeFormat.hasMinutes());
    QVERIFY(not timeFormat.hasFractions());
}

void TestTimeFormat::testNoDividers()
{
    TimeFormat timeFormat;
    QTime t {0, 0};

    timeFormat.showDividers(false);

    QCOMPARE(timeFormat.format(t), QStringLiteral("000000"));
}

void TestTimeFormat::testEquality()
{
    TimeFormat timeFormat1;
    TimeFormat timeFormat2;

    QCOMPARE(timeFormat1, timeFormat2);
}

void TestTimeFormat::testInequality()
{
    TimeFormat timeFormat1;
    TimeFormat timeFormat2 {true, false};

    QVERIFY(timeFormat1 != timeFormat2);
}

void TestTimeFormat::testOverrideHours()
{
    TimeFormat timeFormat {false, true, TimeFormat::NoFractions};
    QTime t {0, 0};

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00"));

    timeFormat.overrideHours();

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00:00"));
}

void TestTimeFormat::testOverrideMinutes()
{
    TimeFormat timeFormat {false, false, TimeFormat::UpToTenths};
    QTime t {0, 0};

    QCOMPARE(timeFormat.format(t), QStringLiteral("00.0"));

    timeFormat.overrideMinutes();

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00.0"));
}

QTEST_MAIN(TestTimeFormat)
