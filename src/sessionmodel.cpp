/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

SessionModel::SessionModel(QObject *parent) : QAbstractTableModel(parent)
{
    QFile saveFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QLatin1String("/sessions.json"));
    saveFile.open(QIODevice::ReadOnly);

    auto saveData = saveFile.readAll();
    auto saveDoc = QJsonDocument::fromJson(saveData);
    read(saveDoc.object());

    connect(this, &QAbstractTableModel::dataChanged, this, &SessionModel::slotWrite);
    connect(this, &QAbstractTableModel::rowsInserted, this, &SessionModel::slotWrite);
    connect(this, &QAbstractTableModel::rowsRemoved, this, &SessionModel::slotWrite);
}

int SessionModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return SESSION_TAG_NUMBER;
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

     if (role == Qt::DisplayRole) {
        QVariant variant;

        switch (index.column()) {
        case NUMBER:
            variant = QString::number(index.row());
            break;

        case NAME:
            variant = m_sessionList.at(index.row()).name();
            break;

        case DATE:
            variant = m_sessionList.at(index.row()).date();
            break;
            return QVariant::Invalid;

        case NOTE:
            variant = m_sessionList.at(index.row()).note();
            break;
        }

         return variant;
     }
     else if (role == Qt::EditRole && index.column() == NAME) {
         // prevent the disappear of the old value when double-clicking the item
         return m_sessionList.at(index.row()).name();
     }
     else if (role == Qt::EditRole && index.column() == NOTE) {
         return m_sessionList.at(index.row()).note();
     }

     return QVariant::Invalid;
}


QVariant SessionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case NUMBER:
                return i18n("Session #");
                break;

            case NAME:
                return i18n("Name");
                break;

            case DATE:
                return i18n("Date");
                break;

            case NOTE:
                return i18n("Note");
                break;
            }
        }
    }

    return QVariant::Invalid;
}

bool SessionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() and role == Qt::EditRole) {
        if (index.column() == NAME) {
            if (value.toString().isEmpty())
                return false;

            m_sessionList[index.row()].setName(value.toString());
            emit dataChanged(index, index);

            return true;
        }
        else if (index.column() == NOTE) {
            m_sessionList[index.row()].setNote(value.toString());
            emit dataChanged(index, index);

            return true;
        }
    }

    return false;
}

Qt::ItemFlags SessionModel::flags(const QModelIndex& index) const
{
    if (not index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == NAME || index.column() == NOTE)
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

    emit dataChanged(QModelIndex(), QModelIndex());
}

bool SessionModel::isEmpty() const
{
    return m_sessionList.isEmpty();
}

bool SessionModel::isEditable(const QModelIndex& index) const
{
    return index.column() == NAME || index.column() == NOTE;
}

void SessionModel::read(const QJsonObject& json)
{
    auto sessions = json[QLatin1String("sessions")].toArray();

    for (int i = 0; i < sessions.size(); i++) {
        append(Session::fromJson(sessions[i].toObject()));
    }
}

void SessionModel::slotWrite()
{
    QJsonObject json;
    QJsonArray sessions;

    Q_FOREACH (const Session& session, m_sessionList) {
        QJsonObject object;
        session.write(object);
        sessions.append(object);
    }

    json[QLatin1String("sessions")] = sessions;

    QFile saveFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QLatin1String("/sessions.json"));
    saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
}

