/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
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
 * The model can also show the relative time of the lap, computing the difference between two consecutive absolute times.
 */
class LapModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum class Roles
    {
        LapIdRole = Qt::UserRole,
        RelativeTimeRole,
        AbsoluteTimeRole,
        NoteRole,
        LapRole
    };

    explicit LapModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
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
     * Insert a new Lap object to the end of the model.
     * @param lap The new Lap object.
     */
    void append(const Lap& lap);

    /**
     * @return Whether the model does not hold any lap.
     */
    bool isEmpty() const;

    /**
     * @return The index of the column for the given role.
     */
    int columnForRole(Roles role) const;

public slots:

    /**
     * Add a new absolute lap time to the model.
     * @param lapTime The absolute time of the new lap.
     */
    void addLap(const QTime& lapTime);

    /**
     * Clear all the model data
     */
    void clear();

private:

    const QVector<Roles> m_roles {Roles::LapIdRole, Roles::RelativeTimeRole, Roles::AbsoluteTimeRole, Roles::NoteRole};

    QVector<Lap> m_laps;             /** Lap times */
    TimeFormat m_timeFormat;          /** Current lap times format */

    /**
     *  Reload the model data.
     */
    void reload();

    /**
     * @return The role for the given column.
     */
    Roles roleForColumn(int column) const;

    Q_DISABLE_COPY(LapModel)
};


#endif
