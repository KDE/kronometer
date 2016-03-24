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

#ifndef LAPMODEL_H
#define LAPMODEL_H

#include "lap.h"
#include "timeformat.h"

#include <QAbstractTableModel>

class QTime;

/**
 * @brief A LapModel is a Model for lap times.
 * A LapModel holds a list of times. Every time is meant to be the absolute time of a lap.
 * Then the model show the relative time of the lap, computing the difference between two consecutive absolute times.
 */
class LapModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum LapTag
    {
        LapId = 0,           /**< Index of the lap-number column */
        RelativeTime = 1,    /**< Index of the lap relative time column */
        AbsoluteTime = 2,    /**< Index of the lap absolute time column */
        Note = 3             /**< Index of the lap annotation column */
    };

    explicit LapModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    /**
     * Update the lap times format used by the model.
     * @param format The times format to be used.
     */
    void setTimeFormat(const TimeFormat& format);

    /**
     * Retrieve a specific Lap object. The given index must be a valid index in the model.
     * @param lapIndex The index of the lap
     * @return The Lap at the given index
     */
    const Lap& at(int lapIndex);

    /**
     * Insert a new Lap object to the end of the model.
     * @param lap The new Lap object.
     */
    void append(const Lap& lap);

    /**
     * Whether the model is empty.
     * @return true if the model holds at least one lap time, false otherwise.
     */
    bool isEmpty() const;

public slots:

    /**
     * Add a new absolute lap time to the model.
     * @param lapTime The absolute time of the new lap.
     */
    void slotLap(const QTime& lapTime);

    /**
     * Clear all the model data
     */
    void slotClear();

private:

    static const int LAP_TAG_NUMBER = 4;    /** Number of tag/header in the model */

    QVector<Lap> m_laps;             /** Lap times */
    TimeFormat m_timeFormat;          /** Current lap times format */

    /**
     *  Reload the model data.
     */
    void reload();

    Q_DISABLE_COPY(LapModel)
};


#endif
