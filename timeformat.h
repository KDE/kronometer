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
#include <QTime>

/**
 * @brief Interface for a customizable time format.
 * A class implementing this interface manages a time format, extending the format syntax used by QTime class.
 * This time format is customizable by the only public function.
 */
class TimeFormat
{

public:

    virtual ~TimeFormat() {}

    /**
     * Set the time format used internally by the class.
     * @param hours Wheter to show timer hours (0 to 23).
     * @param min Wheter to show timer minutes (00 to 59).
     * @param sec Wheter to show timer seconds (00 to 59).
     * @param tenths Wheter to show timer tenths of second (0 to 9). Ignored if hundredths or msec are true.
     * @param hundredths Wheter to show timer tenths and hundredths of second (00 to 99). Ignored if msec is true.
     * @param msec Wheter to show timer tenths/hundredths of second and milliseconds (000 to 999).
     */
    virtual void setTimeFormat(bool hours, bool min, bool sec, bool tenths, bool hundredths, bool msec) = 0;

protected:

    /**
     * Format the given time to a string representation, extending the QTime format syntax.
     * In particulat, the extension adds the ability to display only tenths of second or tenths and hundredths of second.
     * @param time The time to be formatted
     * @return The string representation to the given time, according to the internal time format.
     */
    virtual QString format(const QTime& time) const
    {
        QString formattedTime;

        if (timeFormat.endsWith("zzz"))
            formattedTime = time.toString(timeFormat);

        else if (timeFormat.endsWith("zz"))
        {
            QString original = time.toString(timeFormat + "z");
            formattedTime = original.left(original.size() - 1);
        }

        else if (timeFormat.endsWith("z"))
        {
            QString original = time.toString(timeFormat + "zz");
            formattedTime = original.left(original.size() - 2);
        }

        else
        {
            formattedTime = time.toString(timeFormat);
        }

        return formattedTime;
    }

    QString timeFormat;     /** Time format string used internally */

};

#endif

