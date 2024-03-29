/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lapmodel.h"

#include <KLocalizedString>

#include <QTime>

LapModel::LapModel(QObject* parent) : QAbstractTableModel(parent)
{}

int LapModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_roles.count();
}

int LapModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_laps.size();
}

QVariant LapModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant::Invalid;
    }

    if (index.row() >= rowCount() || index.row() < 0) {
        return QVariant::Invalid;
    }

    if (role == Qt::DisplayRole) {
        return data(index, Qt::UserRole + index.column());
    }

    switch (static_cast<Roles>(role)) {
    case Roles::LapIdRole:
        return index.row() + 1;
    case Roles::RelativeTimeRole:
        return m_laps.at(index.row()).relativeTime();
    case Roles::AbsoluteTimeRole:
        return m_laps.at(index.row()).absoluteTime();
    case Roles::NoteRole:
        return m_laps.at(index.row()).note();
    case Roles::LapRole:
        return QVariant::fromValue(m_laps.at(index.row()));
    }

    if (role == Qt::EditRole && index.column() == columnForRole(Roles::NoteRole)) {
        // prevent the disappear of the old value when double-clicking the item
        return m_laps.at(index.row()).note();
    }

    return QVariant::Invalid;
}

QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant::Invalid;

    switch (roleForColumn(section)) {
    case Roles::LapIdRole:
        return i18nc("lap number", "Lap #");
    case Roles::RelativeTimeRole:
        return i18nc("@title:column", "Lap Time");
    case Roles::AbsoluteTimeRole:
        return i18nc("@title:column", "Global Time");
    case Roles::NoteRole:
        return i18nc("@title:column", "Note");
    case Roles::LapRole:
        break;
    }

    return QVariant::Invalid;
}

bool LapModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.column() != columnForRole(Roles::NoteRole))
        return false;

    m_laps[index.row()].setNote(value.toString());
    Q_EMIT dataChanged(index, index);

    return true;
}

Qt::ItemFlags LapModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() != columnForRole(Roles::NoteRole))
        return QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void LapModel::setTimeFormat(const TimeFormat& format)
{
    m_timeFormat = format;

    if (!isEmpty()) {
        reload();
    }
}

void LapModel::append(const Lap& lap)
{
    // Append the new row at the end.
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

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

int LapModel::columnForRole(LapModel::Roles role) const
{
    return m_roles.indexOf(role);
}

void LapModel::addLap(const QTime& lapTime)
{
    append(Lap {lapTime});
}

void LapModel::clear()
{
    beginResetModel();
    m_laps.clear();
    endResetModel();
}

void LapModel::reload()
{
    const auto laps = m_laps;
    clear();

    for (const auto& lap : laps) {
        append(lap);
    }
}

LapModel::Roles LapModel::roleForColumn(int column) const
{
    return m_roles.at(column);
}

#include "moc_lapmodel.cpp"
