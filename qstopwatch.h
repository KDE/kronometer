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

#ifndef QSTOPWATCH_H
#define QSTOPWATCH_H

#include <QObject>
#include <QElapsedTimer>

class QTime;
class QTimerEvent;
class QDataStream;
class QDomElement;

/**
 * @brief A Stopwatch class written in Qt.
 * QStopwatch is a simple QObject implementing a real stopwatch.
 * The class provides public slots for start/pause/reset the timer.
 * A slot for lap recording exists too, but the computing of lap times is not a task of this class:
 * QStopwatch simply emits a signal, that the receiver can use to compute lap times.
 */
class QStopwatch : public QObject
{
    Q_OBJECT

public:

    enum Granularity
    {
        MILLISECONDS = 1, /**< Stopwatch refreshed every msec. */
        HUNDREDTHS = 10,  /**< Stopwatch refreshed every 10 msec. */
        TENTHS = 100,     /**< Stopwatch refreshed every 100 msec. */
        SECONDS = 1000   /**< Stopwatch refreshed every sec. */
    };

    explicit QStopwatch(QObject *parent = nullptr);

    /**
     * Set the stopwatch refresh granularity
     * @param g The granularity to be set.
     */
    void setGranularity(Granularity g);

    /**
     * Check is the stopwatch is running
     * @return true if running, false otherwise
     */
    bool isRunning() const;

    /**
     * Check is the stopwatch is paused
     * @return true if paused, false otherwise
     */
    bool isPaused() const;

    /**
     * Check is the stopwatch is inactive
     * @return true if inactive, false otherwise
     */
    bool isInactive() const;

    /**
     * Serialize the stopwatch on the given output stream.
     * Only a paused stopwatch is meant to be serialized.
     * @param out The serialization output stream
     * @return true if the serialization succeeds (i.e. the stopwatch is paused), false otherwise
     */
    bool serialize(QDataStream& out);

    /**
     * De-serialize the stopwatch from the given input stream.
     * Only an inactive stopwatch is meant to be serialized.
     * @param in The serialization input stream
     * @return true if the deserialization succeeds (i.e. the stopwatch is inactive), false otherwise
     */
    bool deserialize(QDataStream& in);

    /**
     * Serialize the stopwatch on the given XML DOM element.
     * The serialization is implemented by adding an attribute (with the given name) to the element.
     * Only a paused stopwatch is meant to be serialized.
     * @param element The XML DOM element to be used as serialization output.
     * @param attributeName The name of the attribute to be added to the element.
     * @return true if the serialization succeeds (i.e. the stopwatch is paused), false otherwise
     */
    bool serialize(QDomElement& element, const QString& attributeName);

    /**
     * De-serialize the stopwatch from the given XML DOM element.
     * The deserialization is implemented by reading an attribute (with the given name) from the element.
     * Only an inactive stopwatch is meant to be serialized.
     * @param element The XML DOM element to be used as serialization input.
     * @param attributeName The name of the attribute to be read.
     * @return true if the deserialization succeeds (i.e. the stopwatch is inactive and the attribute is valid), false otherwise
     */
    bool deserialize(QDomElement& element, const QString& attributeName);

public slots:

    /**
     * Start the stopwatch, if inactive or paused.
     */
    void start();

    /**
     * Pause the stopwatch, if running.
     */
    void pause();

    /**
     * Reset the stopwatch to the inactive state.
     */
    void reset();

    /**
     * Tells the stopwatch to emits a signal with the last lap time.
     */
    void lap();

signals:

    /**
     * Emits a signal with the last lap *absolute* time.
     * This class does not compute *relatives* lap times.
     * You can compute them simply by the difference between consecutives absolute times.
     * @param lapTime The absolute time of the last lap.
     */
    void lap(const QTime& lapTime);

    /**
     * Emits a signal with the current stopwatch time.
     * @param t Current stopwatch time.
     */
    void time(const QTime& t);

protected:

    void timerEvent(QTimerEvent *event);

private:

    enum class State
    {
        INACTIVE, /**< Inactive stopwatch. */
        RUNNING,  /**< Running stopwatch. */
        PAUSED    /**< Paused stopwatch. */
    };
	
    static const int INACTIVE_TIMER_ID = -1;    /** Used for timerId initialization */

    int timerId;                                /** ID for the QObject timer */
    qint64 accumulator;                         /** milliseconds internal counter */
    State state;                                /** Stopwatch current state */
    Granularity granularity;                    /** Stopwatch current granularity */

    QElapsedTimer elapsedTimer;                 /** Stopwatch core class*/
};



#endif
