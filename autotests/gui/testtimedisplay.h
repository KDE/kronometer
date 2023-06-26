/*
    SPDX-FileCopyrightText: 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTTIMEDISPLAY_H
#define TESTTIMEDISPLAY_H

#include <QTest>

class TestTimeDisplay : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testDefaultWidget();
    void testSetBackgroundColor();
    void testSetTextColor();
};

#endif
