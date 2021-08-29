/*
    SPDX-FileCopyrightText: 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTTIMEDISPLAY_H
#define TESTTIMEDISPLAY_H

#include <QtTest>

class TestTimeDisplay : public QObject
{
    Q_OBJECT

private slots:

    void testDefaultWidget();
    void testSetBackgroundColor();
    void testSetTextColor();
};

#endif
