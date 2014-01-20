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

#ifndef LAP_MODEL_H
#define LAP_MODEL_H

#include <QAbstractTableModel>
#include <QTime>

#include "timeformat.h"

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

    void setTimeFormat(const TimeFormat& format);
	
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

	static const int LAP_TAG_NUMBER = 3;	/** Number of tag/header in the model */
	
	enum LapTag
	{
        NUMBER = 0,     /**< Index of the lap-number column */
        REL_TIME = 1,   /**< Index of the lap relative time column */
        ABS_TIME = 2,   /**< Index of the lap absolute time column */
	};
	
	QList<QTime> timeList;		/** Absolute lap times */
    TimeFormat timeFormat;         /** Current lap times format */
	
    /**
     * Compute the relative time of a lap.
     * @param lapIndex The index of the lap.
     * @return Relative time of the lap, formatted as string.
     */
	QString lapTime(int lapIndex) const;
};




#endif
