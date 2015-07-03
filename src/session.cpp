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

#include "session.h"

#include <QJsonArray>
#include <QJsonObject>

Session::Session(int time, const QDateTime& date) :
    m_time(time),
    m_date(date),
    m_isOutdated(false)
{}

void Session::setName(const QString& name)
{
    m_name = name;
}

void Session::setNote(const QString& note)
{
    m_note = note;
}

void Session::setTime(int time)
{
    m_time = time;
}

void Session::setDate(const QDateTime& date)
{
    m_date = date;
}

void Session::setOutdated(bool outdated)
{
    m_isOutdated = outdated;
}

QString Session::name() const
{
    return m_name;
}

QString Session::note() const
{
    return m_note;
}

int Session::time() const
{
    return m_time;
}

QDateTime Session::date() const
{
    return m_date;
}

bool Session::isOutdated() const
{
    return m_isOutdated;
}

const QList<Lap>& Session::laps() const
{
    return m_laps;
}

bool Session::isEmpty() const
{
    return m_time == 0;
}

void Session::addLap(const Lap& lap)
{
    m_laps.append(lap);
}

void Session::clear()
{
    m_time = 0;
    m_laps.clear();
}

void Session::write(QJsonObject& json) const
{
    json[QLatin1String("name")] = m_name;
    json[QLatin1String("note")] = m_note;
    json[QLatin1String("time")] = m_time;
    json[QLatin1String("date")] = m_date.toString();

    QJsonArray laps;

    foreach (const Lap& lap, m_laps) {
        QJsonObject object;
        lap.write(object);
        laps.append(object);
    }

    json[QLatin1String("laps")] = laps;
}

Session Session::fromJson(const QJsonObject& json)
{
    Session session;

    session.m_name = json[QLatin1String("name")].toString();
    session.m_note = json[QLatin1String("note")].toString();
    session.m_time = json[QLatin1String("time")].toInt();
    session.m_date = QDateTime::fromString(json[QLatin1String("date")].toString());

    auto laps = json[QLatin1String("laps")].toArray();

    for (int i = 0; i < laps.size(); i++) {
        session.addLap(Lap::fromJson(laps[i].toObject()));
    }

    return session;
}

bool Session::operator==(const Session& right) const
{
    return m_date == right.m_date;
}

