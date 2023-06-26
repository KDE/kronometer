/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTTIMEFORMAT_H
#define TESTTIMEFORMAT_H

#include <QTest>

class TestTimeFormat : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testDefaultFormat();
    void testFullFormat();
    void testMinimalFormat();
    void testNoDividers();
    void testEquality();
    void testInequality();
    void testOverrideHours();
    void testOverrideMinutes();
};

#endif
