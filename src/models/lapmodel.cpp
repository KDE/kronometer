/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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

    return m_columns.count();
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

    auto column = static_cast<Column>(index.column());

    if (role == Qt::DisplayRole) {
        QVariant variant;

        switch (column) {
        case Column::LapId:
            variant = QString::number(index.row() + 1);
            break;
        case Column::RelativeTime:
            variant = m_laps.at(index.row()).relativeTime();
            break;
        case Column::AbsoluteTime:
            variant = m_laps.at(index.row()).absoluteTime();
            break;
        case Column::Note:
            variant = m_laps.at(index.row()).note();
            break;
        }

        return variant;
    }

    if (role == Qt::EditRole && column == Column::Note) {
        // prevent the disappear of the old value when double-clicking the item
        return m_laps.at(index.row()).note();
    }

    return QVariant::Invalid;
}

QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole or orientation != Qt::Horizontal)
        return QVariant::Invalid;

    switch (static_cast<Column>(section)) {
    case Column::LapId:
        return i18nc("lap number", "Lap #");
    case Column::RelativeTime:
        return i18nc("@title:column", "Lap Time");
    case Column::AbsoluteTime:
        return i18nc("@title:column", "Global Time");
    case Column::Note:
        return i18nc("@title:column", "Note");
    default:
        return QVariant::Invalid;
    }
}

bool LapModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (not index.isValid() or role != Qt::EditRole)
        return false;

    if (index.column() != static_cast<int>(Column::Note))
        return false;

    m_laps[index.row()].setNote(value.toString());
    emit dataChanged(index, index);

    return true;
}

Qt::ItemFlags LapModel::flags(const QModelIndex& index) const
{
    if (not index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() != static_cast<int>(Column::Note))
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

    // Either the time of the first lap or the time relative to the last lap.
    const auto relativeTime = m_laps.isEmpty() ? lap.time() : m_laps.last().timeTo(lap);

    auto newLap = Lap {lap};
    newLap.setRelativeTime(m_timeFormat.format(relativeTime));
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
    append(Lap {lapTime});
}

void LapModel::slotClear()
{
    beginResetModel();
    m_laps.clear();
    endResetModel();
}

void LapModel::reload()
{
    const auto laps = m_laps;
    slotClear();

    for (const auto& lap : laps) {
        append(lap);
    }
}
