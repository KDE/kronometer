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

#include "qtimeformat.h"

#include <QTime>

QTimeFormat::QTimeFormat(bool h, bool mm, bool ss, bool t, bool hundr, bool msec)
    :
    hour(h),
    min(mm),
    sec(ss)
{
    if (msec) {
        secFraction = SecFraction::MILLISECOND;
    }
    else if (hundr) {
        secFraction = SecFraction::HUNDREDTH;
    }
    else if (t) {
        secFraction = SecFraction::TENTH;
    }

    setupFormat();
}

QString QTimeFormat::format(const QTime& time) const
{
    QString h = formatHours(time);
    QString m = formatMin(time);
    QString s = formatSec(time);
    QString f = formatSecFrac(time);

    return h + m + s + f;
}

QString QTimeFormat::formatHours(const QTime& time) const
{
    if (not hour) {
        return QString();
    }

    return time.toString(hourFormat);
}

QString QTimeFormat::formatMin(const QTime& time) const
{
    if (not min) {
        return QString();
    }

    return time.toString(minFormat);
}

QString QTimeFormat::formatSec(const QTime& time) const
{
    if (not sec) {
        return QString();
    }

    return time.toString(secFormat);
}

QString QTimeFormat::formatSecFrac(const QTime& time) const
{
    const QString fractFormat = "zzz";

    if (secFraction == SecFraction::MILLISECOND)
        return time.toString(fractFormat);

    if (secFraction == SecFraction::HUNDREDTH) {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 1);
    }

    if (secFraction == SecFraction::TENTH) {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 2);
    }

    return QString();
}

bool QTimeFormat::isHourEnabled() const
{
    return hour;
}

bool QTimeFormat::isMinEnabled() const
{
    return min;
}

bool QTimeFormat::isSecEnabled() const
{
    return sec;
}

bool QTimeFormat::isSecFracEnabled() const
{
    return secFraction != SecFraction::NONE;
}

bool QTimeFormat::isTenthEnabled() const
{
    return secFraction == SecFraction::TENTH;
}

bool QTimeFormat::isHundredthEnabled() const
{
    return secFraction == SecFraction::HUNDREDTH;
}

bool QTimeFormat::isMSecEnabled() const
{
    return secFraction == SecFraction::MILLISECOND;
}

void QTimeFormat::setupFormat()
{
    if (hour) {
        if (min or sec or secFraction != SecFraction::NONE) {
            hourFormat = "h:";
        }
        else {
            hourFormat = "h";
        }
    }

    if (min) {
        if (sec or secFraction != SecFraction::NONE) {
            minFormat = "mm:";
        }
        else {
            minFormat = "mm";
        }
    }

    if (sec) {
        if (secFraction != SecFraction::NONE) {
            secFormat = "ss.";
        }
        else {
            secFormat = "ss";
        }
    }
}
