/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#include "sessionmodel.h"

#include <KLocalizedString>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

SessionModel::SessionModel(QObject *parent) : QAbstractTableModel(parent)
{
    QFile saveFile {QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QLatin1String("/sessions.json")};
    if (saveFile.exists()) {
        if (saveFile.open(QIODevice::ReadOnly)) {
            auto saveData = saveFile.readAll();
            auto saveDoc = QJsonDocument::fromJson(saveData);
            read(saveDoc.object());
        } else {
            qDebug().nospace() << "cannot open " << saveFile.fileName() << ": " << saveFile.errorString();
        }
    }

    connect(this, &QAbstractTableModel::dataChanged, this, &SessionModel::slotWrite);
    connect(this, &QAbstractTableModel::rowsInserted, this, &SessionModel::slotWrite);
    connect(this, &QAbstractTableModel::rowsRemoved, this, &SessionModel::slotWrite);
}

int SessionModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_columns.count();
}

int SessionModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_sessionList.size();
}

QVariant SessionModel::data(const QModelIndex& index, int role) const
{
    if (not index.isValid()) {
        return QVariant::Invalid;
    }

    if (index.row() >= m_sessionList.size() || index.row() < 0) {
        return QVariant::Invalid;
    }

    auto column = static_cast<Column>(index.column());

    if (role == Qt::DisplayRole) {
        auto variant = QVariant {};

        switch (column) {
        case Column::SessionId:
            variant = QString::number(index.row() + 1);
            break;
        case Column::Name:
            variant = m_sessionList.at(index.row()).name();
            break;

        case Column::Date:
            variant = m_sessionList.at(index.row()).date();
            break;
            return QVariant::Invalid;
        case Column::Note:
            variant = m_sessionList.at(index.row()).note();
            break;
        }

        return variant;
    }

    if (role == Qt::EditRole && column == Column::Name) {
        // prevent the disappear of the old value when double-clicking the item
        return m_sessionList.at(index.row()).name();
    }

    if (role == Qt::EditRole && column == Column::Note) {
        return m_sessionList.at(index.row()).note();
    }

    return QVariant::Invalid;
}


QVariant SessionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole or orientation != Qt::Horizontal)
        return QVariant::Invalid;

    switch (static_cast<Column>(section)) {
    case Column::SessionId:
        return i18nc("session number", "Session #");
    case Column::Name:
        return i18n("Name");
    case Column::Date:
        return i18n("Date");
    case Column::Note:
        return i18n("Note");
    default:
        return QVariant::Invalid;
    }
}

bool SessionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (not index.isValid() or role != Qt::EditRole)
        return false;

    if (index.column() == static_cast<int>(Column::Name)) {
        if (value.toString().isEmpty())
            return false;

        m_sessionList[index.row()].setName(value.toString());
        emit dataChanged(index, index);

        return true;
    }

    if (index.column() == static_cast<int>(Column::Note)) {
        m_sessionList[index.row()].setNote(value.toString());
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

Qt::ItemFlags SessionModel::flags(const QModelIndex& index) const
{
    if (not index.isValid())
        return Qt::ItemIsEnabled;

    if (isEditable(index))
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    return QAbstractTableModel::flags(index);
}

bool SessionModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 || row + count - 1 >= m_sessionList.size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++) {
        m_sessionList.removeAt(row);
    }

    endRemoveRows();

    return true;
}

const Session& SessionModel::at(int sessionIndex)
{
    return m_sessionList.at(sessionIndex);
}

void SessionModel::append(const Session& session)
{
    beginInsertRows(QModelIndex(), m_sessionList.size(), m_sessionList.size());    // i.e. append the new row at the table end
    m_sessionList.append(session);
    endInsertRows();
}

void SessionModel::update(const Session& session)
{
    int i = m_sessionList.indexOf(session);

    if (i == -1)
        return;

    m_sessionList[i] = session;
    m_sessionList[i].setDate(QDateTime::currentDateTime());

    emit dataChanged({}, {});
}

bool SessionModel::isEmpty() const
{
    return m_sessionList.isEmpty();
}

bool SessionModel::isEditable(const QModelIndex& index) const
{
    auto column = static_cast<Column>(index.column());
    return column == Column::Name || column == Column::Note;
}

void SessionModel::read(const QJsonObject& json)
{
    const auto sessions = json[QStringLiteral("sessions")].toArray();
    for (const auto& session : sessions) {
        append(Session::fromJson(session.toObject()));
    }
}

void SessionModel::slotWrite()
{
    auto sessions = QJsonArray {};
    for (const auto& session : m_sessionList) {
        auto object = QJsonObject {};
        session.write(object);
        sessions.append(object);
    }

    auto json = QJsonObject {};
    json[QStringLiteral("sessions")] = sessions;

    QFile saveFile {QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QLatin1String("/sessions.json")};
    saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    auto saveDoc = QJsonDocument {json};
    saveFile.write(saveDoc.toJson());
}

