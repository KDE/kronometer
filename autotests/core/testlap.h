/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTLAP_H
#define TESTLAP_H

#include <QtTest>

class TestLap : public QObject
{
    Q_OBJECT

private slots:

    void testDefaultLap();
    void testLapTime();
    void testRelativeTime();
    void testAbsoluteTime();
    void testNote();
    void testRawData();
    void testTimeTo();
};

#endif
