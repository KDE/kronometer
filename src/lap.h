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

#ifndef LAP_H
#define LAP_H

#include <QJsonObject>
#include <QString>
#include <QTime>

/**
 * @brief A Lap is a specific time instant.
 * This class is a wrapper for a QTime object and some strings, which are useful to describe it.
 */
class Lap
{

public:

    explicit Lap(const QTime& lap = QTime(0, 0));

    /**
     * The specific lap's time
     * @return The underlying lap's time object
     */
    QTime time() const;

    /**
     * Set the lap's relative time
     * @param rel The string to be set as relative time
     */
    void setRelativeTime(const QString& rel);

    /**
     * The relative lap time
     * @return String representation of the relative lap time
     */
    QString relativeTime() const;

    /**
     * Set the lap's absolute time
     * @param abs The string to be set as absolute lap time
     */
    void setAbsoluteTime(const QString& abs);

    /**
     * The absolute lap time
     * @return String representation of the absolute lap time
     */
    QString absoluteTime() const;

    /**
     * Set the lap's annotation
     * @param note The note to be set
     */
    void setNote(const QString& note);

    /**
     * The lap's annotation
     * @return The lap's annotation
     */
    QString note() const;

    /**
     * Whether the lap has an annotation
     * @return True if there is a note, false otherwise
     */
    bool hasNote() const;

    /**
     * The underlying lap's raw data
     * @return Lap's raw data counter
     */
    qint64 raw() const;

    void write(QJsonObject& json) const;

    static Lap fromJson(const QJsonObject& json);

    /**
     * Create a new Lap object from raw data
     * @param rawData The raw data counter of the new Lap
     * @return A new Lap object created from the given raw data
     */
    static Lap fromRawData(qint64 rawData);

private:

    QTime m_time;      /**  The specific lap time */
    QString m_relativeTime;    /** String representation of the relative lap time, i.e. compared to another lap */
    QString m_absoluteTime;    /** String representation of the specific (absolute) lap time */
    QString m_note;    /** Custom lap annotation */
};

#endif
