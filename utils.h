#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTime>

namespace utils
{

    /**
     * Format the given time to a string representation, partially changing the QTime format syntax.
     * In particular, the changes add the ability to display only tenths of second or tenths and hundredths of second.
     * @param format The format string. It uses the same syntax used by QTime class with few differences:
     * z => display only tenths of second (0 to 9)
     * zz => display tenths and hundredths of second, with a leading zero (00 to 99)
     * zzz => display also milliseconds, with leading zeroes (000 to 999)
     * @param time The time to be formatted
     * @return The string representation to the given time, according to the given format.
     */
    QString formatTime(const QString& format, const QTime& time);
}

#endif
