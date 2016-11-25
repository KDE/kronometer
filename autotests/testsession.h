/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#ifndef TESTSESSION_H
#define TESTSESSION_H

#include <QtTest>

class TestSession : public QObject
{
    Q_OBJECT

private slots:

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
