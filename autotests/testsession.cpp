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

#include "testsession.h"
#include "session.h"

void TestSession::testDefaultSession()
{
    Session session;

    QCOMPARE(session.time(), 0);
    QVERIFY(session.isEmpty());
    QVERIFY(not session.isOutdated());
    QVERIFY(session.laps().isEmpty());
    QVERIFY(session.name().isNull());
    QVERIFY(session.note().isEmpty());
}

void TestSession::testTime()
{
    int time = 1000;
    Session session {time};

    QCOMPARE(session.time(), time);
}

void TestSession::testName()
{
    Session session;
    auto test = QLatin1String("test");

    session.setName(test);

    QCOMPARE(session.name(), test);
}

void TestSession::testNote()
{
    Session session;
    auto test = QLatin1String("test");

    session.setNote(test);

    QCOMPARE(session.note(), test);
}

void TestSession::testDate()
{
    auto date = QDateTime::currentDateTime();
    Session session {0, date};

    QCOMPARE(session.date(), date);
}

void TestSession::testLaps()
{
    Session session;

    const auto laps = QVector<Lap> {Lap(QTime(1, 30)), Lap(QTime(2, 0)), Lap(QTime(1, 45))};
    foreach (const Lap& lap, laps) {
        session.addLap(lap);
    }

    QCOMPARE(session.laps().size(), laps.size());
}

void TestSession::testEquality()
{
    auto date = QDateTime::currentDateTime();

    Session session1 {0, date};
    Session session2 {1000, date};

    QCOMPARE(session1, session2);
}

void TestSession::testInequality()
{
    Session session1 {0, QDateTime::currentDateTime()};
    QTest::qSleep(100);
    Session session2 {0, QDateTime::currentDateTime()};

    QVERIFY(session1 != session2);
}

void TestSession::testJson()
{
    auto date = QDateTime::currentDateTime();
    Session session1 {1000, QDateTime::fromString(date.toString(Qt::ISODate), Qt::ISODate)};
    session1.setName(QStringLiteral("test-name"));
    session1.setNote(QStringLiteral("test-note"));

    const auto laps = QVector<Lap> {Lap(QTime(1, 30)), Lap(QTime(2, 0)), Lap(QTime(1, 45))};
    foreach (const Lap& lap, laps) {
        session1.addLap(lap);
    }

    QJsonObject json;
    session1.write(json);

    Session session2 = Session::fromJson(json);

    QCOMPARE(session1, session2);
    QCOMPARE(session1.time(), session2.time());
    QCOMPARE(session1.name(), session2.name());
    QCOMPARE(session1.note(), session2.note());
    QCOMPARE(session1.laps().size(), session2.laps().size());
}

QTEST_MAIN(TestSession)
