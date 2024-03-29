/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTSESSION_H
#define TESTSESSION_H

#include <QTest>

class TestSession : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testDefaultSession();
    void testTime();
    void testName();
    void testNote();
    void testDate();
    void testLaps();
    void testEquality();
    void testInequality();
    void testJson();
};

#endif
