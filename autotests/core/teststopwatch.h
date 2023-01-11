/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTSTOPWATCH_H
#define TESTSTOPWATCH_H

#include <QtTest>

class TestStopwatch : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testInactive();
    void testRunning();
    void testPaused();
    void testReset();
    void testInitialize();
};

#endif
