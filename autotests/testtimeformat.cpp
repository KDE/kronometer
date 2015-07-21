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
    QTime t(0, 0);

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00.00"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::UpToHundredths);
    QVERIFY(not timeFormat.isHourEnabled());
    QVERIFY(timeFormat.isMinEnabled());
    QVERIFY(timeFormat.isFractionEnabled());
}

void TestTimeFormat::testFullFormat()
{
    TimeFormat timeFormat(true, true, TimeFormat::UpToMilliseconds);
    QTime t(0, 0);

    QCOMPARE(timeFormat.format(t), QStringLiteral("00:00:00.000"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::UpToMilliseconds);
    QVERIFY(timeFormat.isHourEnabled());
    QVERIFY(timeFormat.isMinEnabled());
    QVERIFY(timeFormat.isFractionEnabled());
}

void TestTimeFormat::testMinimalFormat()
{
    TimeFormat timeFormat(false, false, TimeFormat::NoFractions);
    QTime t(0, 0);

    QCOMPARE(timeFormat.format(t), QStringLiteral("00"));
    QCOMPARE(timeFormat.secondFractions(), TimeFormat::NoFractions);
    QVERIFY(not timeFormat.isHourEnabled());
    QVERIFY(not timeFormat.isMinEnabled());
    QVERIFY(not timeFormat.isFractionEnabled());
}

void TestTimeFormat::testNoDividers()
{
    TimeFormat timeFormat;
    QTime t(0, 0);

    timeFormat.showDividers(false);

    QCOMPARE(timeFormat.format(t), QStringLiteral("000000"));
}

QTEST_MAIN(TestTimeFormat)
