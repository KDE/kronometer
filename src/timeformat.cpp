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

TimeFormat::TimeFormat(bool showHours, bool showMinutes, TimeFormat::FractionType fractionType) :
    m_showHours(showHours),
    m_showMinutes(showMinutes),
    m_showDividers(true),
    m_fractionType(fractionType)
{
    setupFormat();
}

QString TimeFormat::format(const QTime& time) const
{
    auto h = formatHour(time);
    auto m = formatMin(time);
    auto s = formatSec(time);
    auto f = formatSecFrac(time);

    return h + m + s + f;
}

QString TimeFormat::formatHour(const QTime& time) const
{
    if (not m_showHours) {
        return QString();
    }

    return time.toString(m_hourFormat);
}

QString TimeFormat::formatMin(const QTime& time) const
{
    if (not m_showMinutes) {
        return QString();
    }

    return time.toString(m_minFormat);
}

QString TimeFormat::formatSec(const QTime& time) const
{
    return time.toString(m_secFormat);
}

QString TimeFormat::formatSecFrac(const QTime& time) const
{
    auto fracFormat = QStringLiteral("zzz");
    QString temp;

    switch (m_fractionType) {
    case UpToTenths:
        temp = time.toString(fracFormat);
        return temp.left(temp.size() - 2);
    case UpToHundredths:
        temp = time.toString(fracFormat);
        return temp.left(temp.size() - 1);
    case UpToMilliseconds:
        return time.toString(fracFormat);
    default:
        return QString();
    }
}

bool TimeFormat::isHourEnabled() const
{
    return m_showHours;
}

bool TimeFormat::isMinEnabled() const
{
    return m_showMinutes;
}

bool TimeFormat::isSecFracEnabled() const
{
    return m_fractionType != NoFractions;
}

bool TimeFormat::isTenthEnabled() const
{
    return m_fractionType == UpToTenths;
}

bool TimeFormat::isHundredthEnabled() const
{
    return m_fractionType == UpToHundredths;
}

bool TimeFormat::isMSecEnabled() const
{
    return m_fractionType == UpToMilliseconds;
}

void TimeFormat::showDividers(bool show)
{
    m_showDividers = show;
    setupFormat();
}

void TimeFormat::setupFormat()
{
    if (m_showHours) {
        if (m_showDividers) {
            m_hourFormat = QStringLiteral("hh:");
        }
        else {
            m_hourFormat = QStringLiteral("hh");
        }
    }

    if (m_showMinutes) {
        if (m_showDividers) {
            m_minFormat = QStringLiteral("mm:");
        }
        else {
            m_minFormat = QStringLiteral("mm");
        }
    }

    if (m_showDividers and m_fractionType != NoFractions) {
        m_secFormat = QStringLiteral("ss.");
    }
    else {
        m_secFormat = QStringLiteral("ss");
    }
}
