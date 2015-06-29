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

#include <QCoreApplication>
#include <QTimerEvent>

Stopwatch::Stopwatch(QObject *parent) :
    QObject(parent),
    zero(0, 0),
    m_timerId(INACTIVE_TIMER_ID),
    m_state(State::INACTIVE),
    m_granularity(HUNDREDTHS)
{}

void Stopwatch::setGranularity(Granularity g)
{
    m_granularity = g;
}

bool Stopwatch::isRunning() const
{
    return m_state == State::RUNNING;
}

bool Stopwatch::isPaused() const
{
    return m_state == State::PAUSED;
}

bool Stopwatch::isInactive() const
{
    return m_state == State::INACTIVE;
}

qint64 Stopwatch::raw() const
{
    return m_accumulator;
}

bool Stopwatch::initialize(qint64 rawData)
{
    if (m_state != State::INACTIVE or rawData <= 0) {
        return false;
    }

    m_accumulator = rawData;
    m_state = State::PAUSED;
    emit time(zero.addMSecs(m_accumulator));  // it signals that has been deserialized and can be resumed

    return true;
}

void Stopwatch::slotStart()
{
    if (m_state == State::INACTIVE) {
        m_accumulator = 0;
        m_elapsedTimer.start();

        if (m_timerId == INACTIVE_TIMER_ID) {
            m_timerId = startTimer(m_granularity);
        }
    }
    else if (m_state == State::PAUSED) {
        m_elapsedTimer.restart();
        m_timerId = startTimer(m_granularity);
    }

    m_state = State::RUNNING;
}

void Stopwatch::slotPause()
{
    if (m_elapsedTimer.isValid()) {
        m_accumulator += m_elapsedTimer.elapsed();
    }

    m_elapsedTimer.invalidate();
    m_state = State::PAUSED;
}

void Stopwatch::slotReset()
{
    m_elapsedTimer.invalidate();          // if state is running, it will emit a zero time at next timerEvent() call
    QCoreApplication::processEvents();
    emit time(zero);
    m_state = State::INACTIVE;
}

void Stopwatch::slotLap()
{
    qint64 lapTime = 0;

    lapTime += m_accumulator;

    if (m_elapsedTimer.isValid()) {
        lapTime += m_elapsedTimer.elapsed();
    }

    emit lap(zero.addMSecs(lapTime));
}

void Stopwatch::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_timerId) {      // forward undesired events
        QObject::timerEvent(event);
        return;
    }

    qint64 t = 0;

    t += m_accumulator;

    if (m_elapsedTimer.isValid()) {
        t += m_elapsedTimer.elapsed();
        emit time(zero.addMSecs(t));
    }
    else {
        killTimer(m_timerId);
        m_timerId = INACTIVE_TIMER_ID;
    }
}
