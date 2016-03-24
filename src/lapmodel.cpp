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

#include <KLocalizedString>

#include <QTime>

LapModel::LapModel(QObject* parent) : QAbstractTableModel(parent)
{}

int LapModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return LAP_TAG_NUMBER;
}

int LapModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_laps.size();
}

QVariant LapModel::data(const QModelIndex& index, int role) const
{
    if (not index.isValid()) {
        return QVariant::Invalid;
    }

    if (index.row() >= m_laps.size() || index.row() < 0) {
        return QVariant::Invalid;
    }

    if (role == Qt::DisplayRole) {
        QVariant variant;

        switch (index.column()) {
        case LapId:
            variant = QString::number(index.row() + 1);
            break;
        case RelativeTime:
            variant = m_laps.at(index.row()).relativeTime();
            break;
        case AbsoluteTime:
            variant = m_laps.at(index.row()).absoluteTime();
            break;
        case Note:
            variant = m_laps.at(index.row()).note();
            break;
        }

        return variant;
    }

    else if (role == Qt::EditRole && index.column() == Note) {
        // prevent the disappear of the old value when double-clicking the item
        return m_laps.at(index.row()).note();
    }

    return QVariant::Invalid;
}

QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole or orientation != Qt::Horizontal)
        return QVariant::Invalid;

    switch (section) {
    case LapId:
        return i18nc("lap number", "Lap #");
    case RelativeTime:
        return i18nc("@title:column", "Lap time");
    case AbsoluteTime:
        return i18nc("@title:column", "Global time");
    case Note:
        return i18nc("@title:column", "Note");
    default:
        return QVariant::Invalid;
    }
}

bool LapModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() and role == Qt::EditRole) {
        if (index.column() == Note) {
            m_laps[index.row()].setNote(value.toString());
            emit dataChanged(index, index);

            return true;
        }
    }

    return false;
}

Qt::ItemFlags LapModel::flags(const QModelIndex& index) const
{
    if (not index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() != Note)
        return QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void LapModel::setTimeFormat(const TimeFormat& format)
{
    m_timeFormat = format;

    if (not isEmpty()) {
        reload();
    }
}

const Lap& LapModel::at(int lapIndex)
{
    return m_laps.at(lapIndex);
}

void LapModel::append(const Lap& lap)
{
    // Append the new row at the end.
    beginInsertRows(QModelIndex(), m_laps.size(), m_laps.size());

    QString relTime;

    // to compute a relative time we need an older lap entry
    if (not m_laps.isEmpty()) {
        relTime = m_timeFormat.format(m_laps.last().timeTo(lap));
    }
    else {  // first lap entry
        relTime = m_timeFormat.format(lap.time());
    }

    Lap newLap(lap);
    newLap.setRelativeTime(relTime);
    newLap.setAbsoluteTime(m_timeFormat.format(newLap.time()));

    m_laps.append(newLap);
    endInsertRows();
}

bool LapModel::isEmpty() const
{
    return m_laps.isEmpty();
}

void LapModel::slotLap(const QTime& lapTime)
{
    append(Lap(lapTime));
}

void LapModel::slotClear()
{
    beginResetModel();
    m_laps.clear();
    endResetModel();
}

void LapModel::reload()
{
    auto tmp = m_laps;
    slotClear();

    foreach (const Lap& l, tmp) {
        append(l);
    }
}
