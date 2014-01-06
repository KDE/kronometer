/*
	Copyright (C) 20014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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

class QStopwatch : public QWidget
{
	Q_OBJECT

public:
	
	enum class State 
	{
		INACTIVE, 
		RUNNING, 
		PAUSED
	};
	
	enum Granularity
	{
		MILLISECONDS = 1,
		HUNDREDTHS = 10,
		TENTHS = 100,
		SECONDS = 1000,
	};

	explicit QStopwatch(QWidget *parent = 0);
	
    virtual ~QStopwatch();
	
	QString format() const;
	
public slots:
	
	void start();
	void pause();
	void reset();
	
protected:
	
	void timerEvent(QTimerEvent *event);
	
private:
	
	static const int INACTIVE_TIMER_ID = -1;	/** Used for timerId initialization */
	
	int timerId;								/** ID for the QObject timer */
	qint64 accumulator;							/** milliseconds internal counter */
	
	QLabel *timeLabel;							
	QElapsedTimer elapsedTimer;
	
	QString timeFormat;							/** Time format string used internally */
	QString timeFormatMsg;						/** Time format message displayed in the UI */
	
	State state;
	Granularity granularity;
	
	void initTimeLabel();
};



#endif