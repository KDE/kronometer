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

#include "lapmodel.h"

#include <KLocale>

#include <QTime>
#include <QDomElement>

LapModel::LapModel(QObject* parent): QAbstractTableModel(parent) {}

int LapModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return LAP_TAG_NUMBER;
}

int LapModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return lapList.size();
}

QVariant LapModel::data(const QModelIndex& index, int role) const
{
     if (!index.isValid()) {
         return QVariant::Invalid;   
     }

     if (index.row() >= lapList.size() || index.row() < 0) {
         return QVariant::Invalid;
     }

     if (role == Qt::DisplayRole) {
        QVariant variant;

        switch (index.column()) {
        case NUMBER:
            variant = QString::number(index.row());
            break;

        case REL_TIME:
            variant = lapList.at(index.row()).relativeTime();
            break;

        case ABS_TIME:
            variant = lapList.at(index.row()).absoluteTime();
            break;

        case NOTE:
            variant = lapList.at(index.row()).note();
            break;
        }

         return variant;
     }

     else if (role == Qt::EditRole && index.column() == NOTE) {
         // prevent the disappear of the old value when double-clicking the item
         QVariant variant = lapList.at(index.row()).note();
         return variant;
     }

     return QVariant::Invalid;
}


QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case NUMBER:
                return i18n("Lap #");
                break;

            case REL_TIME:
                return i18n("Lap time");
                break;

            case ABS_TIME:
                return i18n("Global time");
                break;

            case NOTE:
                return i18n("Note");
                break;
            }
        }
    }

    return QVariant::Invalid;
}

bool LapModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() and role == Qt::EditRole) {
        if (index.column() == NOTE) {
            lapList[index.row()].setNote(value.toString());
            emit dataChanged(index, index);

            return true;
        }
    }

    return false;
}

Qt::ItemFlags LapModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == NOTE)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    return QAbstractTableModel::flags(index);
}

void LapModel::setTimeFormat(const TimeFormat &format)
{
    timeFormat = format;
}

bool LapModel::lapToXml(QDomElement& element, const QString& attributeName, int lapIndex)
{
//    if (lapIndex < 0 or lapIndex >= timeList.size() or attributeName.isEmpty()) {
//        return false;
//    }

//    QTime zero(0, 0);

//    element.setAttribute(attributeName, zero.msecsTo(timeList.at(lapIndex)));

    Q_UNUSED(element);
    Q_UNUSED(attributeName);
    Q_UNUSED(lapIndex);

    return true;
}

bool LapModel::lapFromXml(const QDomElement& element, const QString& attributeName)
{
//    if (attributeName.isEmpty()) {
//        return false;
//    }

//    QString attributeValue = element.attribute(attributeName);
//    qint64 milliseconds = attributeValue.toLongLong();

//    if (milliseconds == 0) {
//        return false;  // invalid attribute name or value
//    }

//    QTime t(0, 0);
//    t = t.addMSecs(milliseconds);
//    beginInsertRows(QModelIndex(),timeList.size(),timeList.size());		// i.e. append the new row at table end
//    timeList.append(t);
//    endInsertRows();

    Q_UNUSED(element);
    Q_UNUSED(attributeName);

    return true;
}

QString LapModel::relativeLapTime(int lapIndex) const
{
//    if (lapIndex < 0 or lapIndex >= timeList.size()) {
//        return QString();
//    }

//    QString time;

//    if (timeList.size() > 1 and lapIndex > 0) {     // compute diff only starting from 2nd entry
//        QTime prev = timeList.at(lapIndex - 1);
//        QTime target = timeList.at(lapIndex);
//        QTime diff(0, 0);
//        diff = diff.addMSecs(prev.msecsTo(target));

//        time = timeFormat.format(diff);
//    }
//    else {  // first lap entry
//        time = timeFormat.format(timeList.first());
//    }

//    return time;
    Q_UNUSED(lapIndex);
    return QString();
}

QString LapModel::absoluteLapTime(int lapIndex) const
{
//    if (lapIndex < 0 or lapIndex >= timeList.size()) {
//        return QString();
//    }

//    return timeFormat.format(timeList.at(lapIndex));
    Q_UNUSED(lapIndex);
    return QString();
}

bool LapModel::isEmpty() const
{
    return lapList.isEmpty();
}

void LapModel::onLap(const QTime& lapTime)
{
    beginInsertRows(QModelIndex(),lapList.size(),lapList.size());		// i.e. append the new row at table end

    Lap lap(lapTime);
    QString relTime;

    if (lapList.size() >= 1) {     // computing the diff needs at least one previous entry
        QTime prev = lapList.last().time();
        QTime target = lap.time();
        QTime diff(0, 0);
        diff = diff.addMSecs(prev.msecsTo(target));

        relTime = timeFormat.format(diff);
    }
    else {  // first lap entry
        relTime = timeFormat.format(lap.time());
    }

    lap.setRelativeTime(relTime);
    lap.setAbsoluteTime(timeFormat.format(lap.time()));

    lapList.append(lap);
    endInsertRows();
}

void LapModel::onClear()
{
    beginResetModel();
    lapList.clear();
    endResetModel();
}
