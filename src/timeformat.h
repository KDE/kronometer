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

#ifndef TIMEFORMAT_H
#define TIMEFORMAT_H

#include <QString>

class QTime;

/**
 * @brief A wrapper for a QTime-like format time string.
 * A TimeFormat is an abstraction for a QTime-like string used for time formats.
 * A TimeFormat can be customized using booleans in the constructor.
 * The QTime-syntax used is the following:
 * "hh:" whether to show hours (00 to 24)
 * "mm:" whether to show minutes (00 to 59)
 * "ss." whether to show seconds (00 to 59). Seconds are always showed.
 * "zzz" whether to show second fractions (tenths or hundredths or milliseconds)
 * An example of time formatted with the complete syntax might be the following: 0:05:38.582
 */
class TimeFormat
{

public:

    enum FractionType
    {
        UpToTenths,          /**< Second fraction is tenths of second. */
        UpToHundredths,      /**< Second fraction is hundrdths of second. */
        UpToMilliseconds,    /**< Second fraction is milliseconds. */
        NoFractions          /**< Second fraction disabled. */
    };

    explicit TimeFormat(bool showHours = false, bool showMinutes = true, FractionType fractionType = UpToHundredths);

    /**
     * Format the given time with the current time format.
     * @param time The time to be formatted.
     * @return The time formatted as string.
     */
    QString format(const QTime& time) const;

    /**
     * Format the given time's hours with the current time format.
     * @param time The time to be formatted.
     * @return The time's hours formatted as string, or empty string if hour is not in the format.
     */
    QString formatHour(const QTime& time) const;

    /**
     * Format the given time's minutes with the current time format.
     * @param time The time to be formatted.
     * @return The time's minutes formatted as string, or empty string if minute is not in the format.
     */
    QString formatMin(const QTime& time) const;

    /**
     * Format the given time's seconds with the current time format.
     * @param time The time to be formatted.
     * @return The time's seconds formatted as string, or empty string if second is not in the format.
     */
    QString formatSec(const QTime& time) const;

    /**
     * Format the given time's second fractions with the current time format.
     * @param time The time to be formatted.
     * @return The time's second fractions formatted as string, or empty string if second fraction is not in the format.
     */
    QString formatSecFrac(const QTime& time) const;

    /**
     * Whether the hour is in the time format.
     * @return true if hour is in the format, false otherwise.
     */
    bool isHourEnabled() const;

    /**
     * Whether the minute is in the time format.
     * @return true if minute is in the format, false otherwise.
     */
    bool isMinEnabled() const;

    /**
     * Whether the second fraction is in the time format.
     * @return true if second fraction is in the format, false otherwise.
     */
    bool isSecFracEnabled() const;

    /**
     * Whether the second fractions in the time format is made by tenths of second.
     * @return true if second fraction is tenths of second, false otherwise.
     */
    bool isTenthEnabled() const;

    /**
     * Whether the second fractions in the time format is made by hundredths of second.
     * @return true if second fraction is hundredths of second, false otherwise.
     */
    bool isHundredthEnabled() const;

    /**
     * Whether the second fractions in the time format is made by milliseconds.
     * @return true if second fraction is milliseconds, false otherwise.
     */
    bool isMSecEnabled() const;

    /**
     * Whether to show the symbols ':' and '.' used as dividers in the time format.
     * @param show true Whether to show the dividers.
     */
    void showDividers(bool show);

private:

    bool m_showHours;             /** Whether hour is in the internal time format */
    bool m_showMinutes;           /** Whether minute is in the internal time format */
    bool m_showDividers;          /** Whether to show the symbols used as dividers */

    QString m_hourFormat;         /** Hour string format */
    QString m_minFormat;          /** Minute string format */
    QString m_secFormat;          /** Secondstring format */
    FractionType m_fractionType;  /** Second fraction internal time format */

    /**
     * Setup the format strings based on the internal formats
     */
    void setupFormat();
};

#endif
