/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include "session.h"

#include <QAbstractTableModel>

/**
 * @brief A SessionModel is a Model for sessions.
 */
class SessionModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum class Roles
    {
        SessionIdRole = Qt::UserRole,
        NameRole,
        DateRole,
        NoteRole,
        SessionRole
    };

    explicit SessionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    /**
     * Insert a new Session object to the end of the model.
     * @param session The new Session object.
     */
    void append(const Session& session);

    /**
     * Update the given session in the model and updates its date.
     * If the session does not belong to the model, nothing happens.
     * @param session The session to be updated.
     */
    void update(Session& session);

    /**
     * @return Whether the model does not hold any session.
     */
    bool isEmpty() const;

    /**
     * Whether the given index refers to an editable value in the model.
     * @param index An index of the model.
     * @return True if the underlying data is editable, false otherwise.
     */
    bool isEditable(const QModelIndex& index) const;

    /**
     * Load the model from the given JSON object.
     * @param json A JSON object.
     */
    void read(const QJsonObject& json);

private Q_SLOTS:

    /**
     * Update the global sessions file with the data in the model.
     */
    void slotWriteData();

private:

    /**
     * @return The index of the column for the given role.
     */
    int columnForRole(Roles role) const;

    /**
     * @return The role for the given column.
     */
    Roles roleForColumn(int column) const;

    /**
     * @return The list of sessions as json array.
     */
    QJsonArray jsonSessions() const;

    const QVector<Roles> m_roles {Roles::SessionIdRole, Roles::NameRole, Roles::DateRole, Roles::NoteRole};

    QVector<Session> m_sessionList;    /** Sessions in the model. */

    Q_DISABLE_COPY(SessionModel)
};


#endif
