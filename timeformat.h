/*
    Copyright (C) 2014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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

#ifndef TIME_FORMAT_H
#define TIME_FORMAT_H

#include <QString>

class QTime;

class TimeFormat
{

public:

    TimeFormat(bool h = false, bool mm = true, bool ss = true, bool t = true, bool hundr = true, bool msec = false);

    QString format(const QTime& time) const;

    QString formatHours(const QTime& time) const;

    QString formatMin(const QTime& time) const;

    QString formatSec(const QTime& time) const;

    QString formatSecFrac(const QTime& time) const;

    bool isHourEnabled() const;

    bool isMinEnabled() const;

    bool isSecEnabled() const;

    bool isSecFracEnabled() const;

    bool isTenthEnabled() const;

    bool isHundredthEnabled() const;

    bool isMSecEnabled() const;

private:

    enum class SecFraction
    {
        NONE,
        TENTH,
        HUNDREDTH,
        MILLISECOND
    };

    bool hour;
    bool min;
    bool sec;
    SecFraction secFraction;

    QString hourFormat;
    QString minFormat;
    QString secFormat;

    void setupFormat();
};

#endif
