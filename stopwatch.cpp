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

#include "stopwatch.h"

#include <QTime>
#include <QTimerEvent>
#include <QDataStream>
#include <QDomElement>
#include <QCoreApplication>

Stopwatch::Stopwatch(QObject *parent) :  QObject(parent), timerId(INACTIVE_TIMER_ID), state(State::INACTIVE), granularity(HUNDREDTHS) {}

void Stopwatch::setGranularity(Granularity g)
{
    granularity = g;
}

bool Stopwatch::isRunning() const
{
    return state == State::RUNNING;
}

bool Stopwatch::isPaused() const
{
    return state == State::PAUSED;
}

bool Stopwatch::isInactive() const
{
    return state == State::INACTIVE;
}

bool Stopwatch::serialize(QDataStream& out)
{
    if (state != State::PAUSED) {
        return false;
    }

    out << accumulator;

    return true;
}

bool Stopwatch::deserialize(QDataStream& in)
{
    if (state != State::INACTIVE) {
        return false;
    }

    in >> accumulator;
    state = State::PAUSED;

    QTime t(0, 0);
    t = t.addMSecs(accumulator);

    emit time(t);  // it signals that has been deserialized and can be resumed

    return true;
}

bool Stopwatch::serialize(QDomElement& element, const QString& attributeName)
{
    if (state != State::PAUSED or attributeName.isEmpty()) {
        return false;
    }

    element.setAttribute(attributeName, accumulator);

    return true;
}

bool Stopwatch::deserialize(QDomElement& element, const QString& attributeName)
{
    if (state != State::INACTIVE or attributeName.isEmpty()) {
        return false;
    }

    QString acc = element.attribute(attributeName);
    accumulator = acc.toLongLong();

    if (accumulator == 0) {
        return false;  // invalid attribute name or value
    }

    state = State::PAUSED;

    QTime t(0, 0);
    t = t.addMSecs(accumulator);

    emit time(t);  // it signals that has been deserialized and can be resumed

    return true;
}

void Stopwatch::onStart()
{
    if (state == State::INACTIVE) {
        accumulator = 0;
        elapsedTimer.start();

        if (timerId == INACTIVE_TIMER_ID) {
            timerId = startTimer(granularity);
        }
    }
    else if (state == State::PAUSED) {
        elapsedTimer.restart();
        timerId = startTimer(granularity);
    }

    state = State::RUNNING;
}

void Stopwatch::onPause()
{
    if (elapsedTimer.isValid()) {
        accumulator += elapsedTimer.elapsed();
    }

    elapsedTimer.invalidate();
    state = State::PAUSED;
}

void Stopwatch::onReset()
{
    elapsedTimer.invalidate();          // if state is running, it will emit a zero time at next timerEvent() call
    QCoreApplication::processEvents();
    emit time(QTime(0,0));
    state = State::INACTIVE;
}

void Stopwatch::onLap()
{

    QTime lapTime(0, 0);

    lapTime = lapTime.addMSecs(accumulator);

    if (elapsedTimer.isValid()) {
        lapTime = lapTime.addMSecs(elapsedTimer.elapsed());
    }

    emit lap(lapTime);
}

void Stopwatch::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != timerId) {      // forward undesired events
        QObject::timerEvent(event);
        return;
    }

    QTime t(0, 0);

    t = t.addMSecs(accumulator);

    if (elapsedTimer.isValid()) {
        t = t.addMSecs(elapsedTimer.elapsed());
    }
    else {
        killTimer(timerId);
        timerId = INACTIVE_TIMER_ID;
    }

    emit time(t);
}
