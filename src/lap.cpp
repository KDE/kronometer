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

#include "lap.h"

Lap::Lap(const QTime& lap) : lapTime(lap) {}

QTime Lap::time() const
{
    return lapTime;
}

void Lap::setRelativeTime(const QString& rel)
{
    relTime = rel;
}

QString Lap::relativeTime() const
{
    return relTime;
}

void Lap::setAbsoluteTime(const QString& abs)
{
    absTime = abs;
}

QString Lap::absoluteTime() const
{
    return absTime;
}

void Lap::setNote(const QString& note)
{
    lapNote = note;
}

QString Lap::note() const
{
    return lapNote;
}
