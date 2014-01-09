/*
	Copyright (C) 20014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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

#include <KLocale>

#include "lapmodel.h"

LapModel::LapModel(QObject* parent): QAbstractTableModel(parent) {}

LapModel::LapModel(QObject* parent, const QString& format) : QAbstractTableModel(parent), timeFormat(format) {}


int LapModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	
	return LAP_TAG_NUMBER;
}

int LapModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return timeList.size();
}

QVariant LapModel::data(const QModelIndex& index, int role) const
{
     if (!index.isValid())
         return QVariant::Invalid;

     if (index.row() >= timeList.size() || index.row() < 0)
         return QVariant::Invalid;
	 
     if (role == Qt::DisplayRole) 
	 {		
		QVariant variant;

		switch (index.column())
		{
			case INDEX:
				variant = QString::number(index.row());
				break;
				
			case REL_TIME:
				variant = lapTime(index.row());
				break;
				
			case ABS_TIME:
				variant = timeList.at(index.row()).toString(timeFormat);
				break;
		}
		 
		 return variant;
     }

     return QVariant::Invalid;
}


QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
				case INDEX:
					return i18n("Lap #");
					break;
					
				case REL_TIME:
					return i18n("Lap time");
					break;
					
				case ABS_TIME:
					return i18n("Global time");
					break;
			}
		}
	}
	
	return QVariant::Invalid;
}

void LapModel::lap(const QTime& lapTime)
{
	beginInsertRows(QModelIndex(),timeList.size(),timeList.size());		// i.e. append the new row at table end
	
	timeList.append(lapTime);
	
	endInsertRows();
}

void LapModel::clear()
{
	beginResetModel();
	
	timeList.clear();
	
	endResetModel();
}

QString LapModel::lapTime(int lapIndex) const
{
	QString time;
	
	if (timeList.size() > 1 and lapIndex > 0)   // compute diff only starting from 2nd entry
	{
		QTime prev = timeList.at(lapIndex - 1);
		QTime target = timeList.at(lapIndex);
		QTime diff(0, 0);
		diff = diff.addMSecs(prev.msecsTo(target));

		time = diff.toString(timeFormat);
	}
	
	else  // first lap entry
	{
		time = timeList.first().toString(timeFormat);
	}
	
	return time;
}




