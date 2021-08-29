/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lap.h"

#include <QJsonObject>

Lap::Lap(const QTime& lap) :
    m_time {lap}
{}

QTime Lap::time() const
{
    return m_time;
}

QTime Lap::timeTo(const Lap& lap) const
{
    if (lap.time() < m_time)
        return {0, 0};

    const auto zero = QTime {0, 0};
    return zero.addMSecs(m_time.msecsTo(lap.time()));
}

void Lap::setRelativeTime(const QString& rel)
{
    m_relativeTime = rel;
}

QString Lap::relativeTime() const
{
    return m_relativeTime;
}

void Lap::setAbsoluteTime(const QString& abs)
{
    m_absoluteTime = abs;
}

QString Lap::absoluteTime() const
{
    return m_absoluteTime;
}

void Lap::setNote(const QString& note)
{
    m_note = note;
}

QString Lap::note() const
{
    return m_note;
}

int Lap::raw() const
{
    const auto zero = QTime {0, 0};
    return zero.msecsTo(m_time);
}

void Lap::write(QJsonObject& json) const
{
    json[QStringLiteral("time")] = raw();
    json[QStringLiteral("reltime")] = m_relativeTime;
    json[QStringLiteral("abstime")] = m_absoluteTime;
    json[QStringLiteral("note")] = m_note;
}

Lap Lap::fromJson(const QJsonObject& json)
{
    auto lap = fromRawData(json[QStringLiteral("time")].toInt());
    lap.m_relativeTime = json[QStringLiteral("reltime")].toString();
    lap.m_absoluteTime = json[QStringLiteral("abstime")].toString();
    lap.m_note = json[QStringLiteral("note")].toString();

    return lap;
}

Lap Lap::fromRawData(int rawData)
{
    if (rawData < 0) {
        return Lap {};
    }

    const auto zero = QTime {0, 0};
    return Lap {zero.addMSecs(rawData)};
}
