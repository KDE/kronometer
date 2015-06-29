/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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

#include "timeformat.h"

#include <QTime>

TimeFormat::TimeFormat(bool h, bool mm, bool ss, bool t, bool hundr, bool msec) :
    m_hour(h),
    m_min(mm),
    m_sec(ss),
    m_dividers(true)
{
    if (msec) {
        m_secFraction = SecFraction::MILLISECOND;
    }
    else if (hundr) {
        m_secFraction = SecFraction::HUNDREDTH;
    }
    else if (t) {
        m_secFraction = SecFraction::TENTH;
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
    if (not m_hour) {
        return QString();
    }

    return time.toString(m_hourFormat);
}

QString TimeFormat::formatMin(const QTime& time) const
{
    if (not m_min) {
        return QString();
    }

    return time.toString(m_minFormat);
}

QString TimeFormat::formatSec(const QTime& time) const
{
    if (not m_sec) {
        return QString();
    }

    return time.toString(m_secFormat);
}

QString TimeFormat::formatSecFrac(const QTime& time) const
{
    const QString fractFormat = QStringLiteral("zzz");

    if (m_secFraction == SecFraction::MILLISECOND)
        return time.toString(fractFormat);

    if (m_secFraction == SecFraction::HUNDREDTH) {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 1);
    }

    if (m_secFraction == SecFraction::TENTH) {
        QString temp = time.toString(fractFormat);
        return temp.left(temp.size() - 2);
    }

    return QString();
}

bool TimeFormat::isHourEnabled() const
{
    return m_hour;
}

bool TimeFormat::isMinEnabled() const
{
    return m_min;
}

bool TimeFormat::isSecEnabled() const
{
    return m_sec;
}

bool TimeFormat::isSecFracEnabled() const
{
    return m_secFraction != SecFraction::NONE;
}

bool TimeFormat::isTenthEnabled() const
{
    return m_secFraction == SecFraction::TENTH;
}

bool TimeFormat::isHundredthEnabled() const
{
    return m_secFraction == SecFraction::HUNDREDTH;
}

bool TimeFormat::isMSecEnabled() const
{
    return m_secFraction == SecFraction::MILLISECOND;
}

void TimeFormat::showDividers(bool show)
{
    m_dividers = show;
    setupFormat();
}

void TimeFormat::setupFormat()
{
    if (m_hour) {
        if (m_dividers and (m_min or m_sec or m_secFraction != SecFraction::NONE)) {
            m_hourFormat = QStringLiteral("hh:");
        }
        else {
            m_hourFormat = QStringLiteral("hh");
        }
    }

    if (m_min) {
        if (m_dividers and (m_sec or m_secFraction != SecFraction::NONE)) {
            m_minFormat = QStringLiteral("mm:");
        }
        else {
            m_minFormat = QStringLiteral("mm");
        }
    }

    if (m_sec) {
        if (m_dividers and (m_secFraction != SecFraction::NONE)) {
            m_secFormat = QStringLiteral("ss.");
        }
        else {
            m_secFormat = QStringLiteral("ss");
        }
    }
}
