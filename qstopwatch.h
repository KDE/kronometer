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

#ifndef Q_STOPWATCH_H
#define Q_STOPWATCH_H

#include <QLabel>
#include <QElapsedTimer>
#include <QTimerEvent>
#include <QTime>

#include "timeformat.h"

/**
 * @brief A Stopwatch class written in Qt.
 * QStopwatch is a simple QWidget implementing a real stopwatch, i.e. a stopwatch using a digital display.
 * The class provides public slots for start/pause/reset the timer.
 * A slot for lap recording exists too, but the computing of lap times is not a task of this class:
 * QStopwatch simply emits a signal, that the receiver can use to compute lap times.
 */
class QStopwatch : public QWidget, public TimeFormat
{
	Q_OBJECT

public:
	
	enum class State 
	{
        INACTIVE, /**< Inactive stopwatch. */
        RUNNING,  /**< Running stopwatch. */
        PAUSED    /**< Paused stopwatch. */
	};
	
	enum Granularity
	{
        MILLISECONDS = 1, /**< Stopwatch refreshed every msec. */
        HUNDREDTHS = 10,  /**< Stopwatch refreshed every 10 msec. */
        TENTHS = 100,     /**< Stopwatch refreshed every 100 msec. */
        SECONDS = 1000,   /**< Stopwatch refreshed every sec. */
	};

    explicit QStopwatch(QWidget *parent = nullptr);
	
    void setTimeFormat(bool hours, bool min, bool sec, bool tenths, bool hundredths, bool msec);

    /**
     * Set a custom font for stopwatch display
     * @param font The custom font to set.
     */
    void setDisplayFont(const QFont& font);
		
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
     * Emits a signal every time that the time format is changed, with a human-readable string as the new time format.
     * @param formatMsg
     */
	void timeFormatChanged(const QString& formatMsg);
	
protected:
	
	void timerEvent(QTimerEvent *event);
	
private:
	
	static const int INACTIVE_TIMER_ID = -1;	/** Used for timerId initialization */
	
	int timerId;								/** ID for the QObject timer */
	qint64 accumulator;							/** milliseconds internal counter */
    State state;                                /** Stopwatch current state */
    Granularity granularity;                    /** Stopwatch current granularity */

    QElapsedTimer elapsedTimer;                 /** Stopwatch core class*/
    QLabel *displayLabel;                       /** Label implementing the digital display with the timer */
	QString timeFormatMsg;						/** Time format message displayed in the UI */
    QFont displayFont;                          /** Font used in timer display */


	
    /**
     * Initialize display label.
     */
    void setupDisplayLabel();

};



#endif
