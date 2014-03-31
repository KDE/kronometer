/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis [dot] angelaccio [at] kdemail [dot] net>

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

#ifndef LAPMODEL_H
#define LAPMODEL_H

#include <QAbstractTableModel>

#include "qtimeformat.h"

class QTime;
class QDomElement;

/**
 * @brief A LapModel is a Model for lap times.
 * A LapModel holds a list of times. Every time is meant to be the absolute time of a lap.
 * Then the model show the relative time of the lap, computing the difference between two consecutive absolute times.
 */
class LapModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit LapModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setTimeFormat(const QTimeFormat& format);

    /**
     * Serialize the suggested lap time to the given XML DOM element.
     * The serialization is implemented by adding an attribute (with the given name) to the element.
     * @param element The XML DOM element to be used as serialization output.
     * @param attributeName The name of the attribute to be added to the element.
     * @param lapIndex the index of the lap time to be serialized.
     * @return true if the serialization succeeds, false otherwise.
     */
    bool lapToXml(QDomElement& element, const QString& attributeName, int lapIndex);

    /**
     * De-serialize the suggested lap time from the given XML DOM element.
     * The deserialization is implemented by reading an attribute (with the given name) from the element.
     * @param element The XML DOM element to be used as deserialization input.
     * @param attributeName The name of the attribute to be read from the element.
     * @param lapIndex the index of the lap time to be deserialized.
     * @return true if the deserialization succeeds, false otherwise.
     */
    bool lapFromXml(const QDomElement& element, const QString& attributeName);

    /**
     * Compute the relative time of a lap.
     * @param lapIndex The index of the lap.
     * @return Relative time of the lap, formatted as string.
     */
    QString relativeLapTime(int lapIndex) const;

    /**
     * Compute the absolute time of a lap.
     * @param lapIndex The index of the lap.
     * @return Absolute time of the lap, formatted as string.
     */
    QString absoluteLapTime(int lapIndex) const;

    /**
     * Whether the model is empty.
     * @return true if the model holds at least one lap time, false otherwise.
     */
    bool isEmpty() const;

    friend QDataStream& operator<<(QDataStream& out, const LapModel& m);
    friend QDataStream& operator>>(QDataStream& in, LapModel& m);

public slots:	

    /**
     * Add a new absolute lap time to the model.
     * @param lapTime The absolute time of the new lap.
     */
    void lap(const QTime& lapTime);

    /**
     * Clear all the model data
     */
    void clear();

private:

    static const int LAP_TAG_NUMBER = 3;    /** Number of tag/header in the model */

    enum LapTag
    {
        NUMBER = 0,     /**< Index of the lap-number column */
        REL_TIME = 1,   /**< Index of the lap relative time column */
        ABS_TIME = 2,   /**< Index of the lap absolute time column */
    };

    QList<QTime> timeList;              /** Absolute lap times */
    QTimeFormat timeFormat;              /** Current lap times format */


};


#endif
