/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
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
    QVERIFY(!timeFormat.hasHours());
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
    QVERIFY(!timeFormat.hasHours());
    QVERIFY(!timeFormat.hasMinutes());
    QVERIFY(!timeFormat.hasFractions());
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
