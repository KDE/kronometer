/*
    Copyright (C) 2016 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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
