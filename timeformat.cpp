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

#include <QTime>

#include "timeformat.h"

TimeFormat::TimeFormat(bool h, bool mm, bool ss, bool t, bool hundr, bool msec)
    :
    hour(h),
    min(mm),
    sec(ss)
{
    if (msec)
    {
        secFraction = SecFraction::MILLISECOND;
    }

    else if (hundr)
    {
        secFraction = SecFraction::HUNDREDTH;
    }

    else if (t)
    {
        secFraction = SecFraction::TENTH;
    }

    setupFormat();
}

QString TimeFormat::format(const QTime& time) const
{
    QString h = formatHours(time);
    QString m = formatMin(time);
    QString s = formatSec(time);
    QString f = formatSecFrac(time);

    return h + m + s + f;
}

QString TimeFormat::formatHours(const QTime& time) const
{
    if (not hour)
    {
        return QString();
    }

    return time.toString(hourFormat);
}

QString TimeFormat::formatMin(const QTime& time) const
{
    if (not min)
    {
        return QString();
    }

    return time.toString(minFormat);
}

QString TimeFormat::formatSec(const QTime& time) const
{
    if (not sec)
    {
        return QString();
    }

    return time.toString(secFormat);
}

QString TimeFormat::formatSecFrac(const QTime& time) const
{
    const QString fractFormat = "zzz";

    if (secFraction == SecFraction::MILLISECOND)
        return time.toString(fractFormat);

    if (secFraction == SecFraction::HUNDREDTH)
    {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 1);
    }

    if (secFraction == SecFraction::TENTH)
    {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 2);
    }

    return QString();
}

bool TimeFormat::isHourEnabled() const
{
    return hour;
}

bool TimeFormat::isMinEnabled() const
{
    return min;
}

bool TimeFormat::isSecEnabled() const
{
    return sec;
}

bool TimeFormat::isSecFracEnabled() const
{
    return secFraction != SecFraction::NONE;
}

bool TimeFormat::isTenthEnabled() const
{
    return secFraction == SecFraction::TENTH;
}

bool TimeFormat::isHundredthEnabled() const
{
    return secFraction == SecFraction::HUNDREDTH;
}

bool TimeFormat::isMSecEnabled() const
{
    return secFraction == SecFraction::MILLISECOND;
}

void TimeFormat::setupFormat()
{
    if (hour)
    {
        if (min or sec or secFraction != SecFraction::NONE)
            hourFormat = "h:";

        else
            hourFormat = "h";
    }

    if (min)
    {
        if (sec or secFraction != SecFraction::NONE)
            minFormat = "mm:";

        else
            minFormat = "mm";
    }

    if (sec)
    {
        if (secFraction != SecFraction::NONE)
            secFormat = "ss.";

        else
            secFormat = "ss";
    }
}
