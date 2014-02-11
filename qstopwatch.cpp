/*
    Copyright (C) 2014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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

#include "qstopwatch.h"

#include <QTime>
#include <QTimerEvent>
#include <QDataStream>
#include <QDomElement>

QStopwatch::QStopwatch(QObject *parent) :  QObject(parent), timerId(INACTIVE_TIMER_ID), state(State::INACTIVE), granularity(HUNDREDTHS) {}

void QStopwatch::setGranularity(Granularity g)
{
    granularity = g;
}

bool QStopwatch::isRunning() const
{
    return state == State::RUNNING;
}

bool QStopwatch::isPaused() const
{
    return state == State::PAUSED;
}

bool QStopwatch::isInactive() const
{
    return state == State::INACTIVE;
}

bool QStopwatch::serialize(QDataStream& out)
{
    if (state != State::PAUSED) {
        return false;
    }

    out << accumulator;

    return true;
}

bool QStopwatch::deserialize(QDataStream& in)
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

bool QStopwatch::serialize(QDomElement& element, const QString& attributeName)
{
    if (state != State::PAUSED or attributeName.isEmpty()) {
        return false;
    }

    element.setAttribute(attributeName, accumulator);

    return true;
}

bool QStopwatch::deserialize(QDomElement& element, const QString& attributeName)
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

void QStopwatch::start()
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

void QStopwatch::pause()
{
    if (elapsedTimer.isValid()) {
        accumulator += elapsedTimer.elapsed();
    }

    elapsedTimer.invalidate();
    state = State::PAUSED;
}

void QStopwatch::reset()
{
    elapsedTimer.invalidate();          // if state is running, it will emit a zero time at next timerEvent() call

    if (state == State::PAUSED) {       // if not, it must be done explicitly
        emit time(QTime(0,0));          // in this way the stopwatch signals that it has been reset
    }

    state = State::INACTIVE;
}

void QStopwatch::lap()
{

    QTime lapTime(0, 0);

    lapTime = lapTime.addMSecs(accumulator);

    if (elapsedTimer.isValid()) {
        lapTime = lapTime.addMSecs(elapsedTimer.elapsed());
    }

    emit lap(lapTime);
}

void QStopwatch::timerEvent(QTimerEvent *event)
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
