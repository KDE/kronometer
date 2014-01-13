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

#include <QHBoxLayout>

#include "qstopwatch.h"

namespace
{
	const QString DEFAULT_TIME_FORMAT = "hh:mm:ss.zzz";
	const QString DEFAULT_TIME_FORMAT_MSG = "h:m:s.ms";
}

QStopwatch::QStopwatch(QWidget *parent) 
	:  
	QWidget(parent), 
	timerId(INACTIVE_TIMER_ID), 
	timeFormat(DEFAULT_TIME_FORMAT), 
	timeFormatMsg(DEFAULT_TIME_FORMAT_MSG),  
	state(State::INACTIVE), 
	granularity(HUNDREDTHS) 
{
	timeLabel = new QLabel(this);
	timeLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(timeLabel);
	
	initTimeLabel();
}

QStopwatch::~QStopwatch()
{

}

void QStopwatch::setTimeFormat(bool hours, bool min, bool sec, bool tenths, bool hundredths, bool msec)
{
	timeFormat = timeFormatMsg = "";	// discard default time format 
	
	if (hours)
	{	
		if (min or sec or tenths or hundredths or msec)
		{
			timeFormat = "h:";
			timeFormatMsg = "hours : ";
		}
		
		else
		{
			timeFormat = "h";
			timeFormatMsg = "hours";
		}	
	}
	
	if (min)
	{
		if (sec or tenths or hundredths or msec)
		{
			timeFormat += "mm:";
			timeFormatMsg += "min : ";
		}
		
		else
		{
			timeFormat += "mm";
			timeFormatMsg += "min";
		}
	}
	
	if (sec)
	{
		if (tenths or hundredths or msec)
		{
			timeFormat += "ss.";
			timeFormatMsg += "sec . ";
		}
		
		else
		{
			timeFormat += "ss";
			timeFormatMsg += "sec";
		}
	}
	
	if (msec)
	{
		timeFormat += "zzz";
		timeFormatMsg += "msec";
		granularity = MILLISECONDS;
	}
	
	else if (hundredths)
	{
		timeFormat += "zzz";
		timeFormat = timeFormat.left(timeFormat.size() - 1);
		timeFormatMsg += "hundreths";
		granularity = HUNDREDTHS;
	}
	
	else if (tenths)
	{
		timeFormat += "zzz";
		timeFormat = timeFormat.left(timeFormat.size() - 2);
		timeFormatMsg += "tenths";
		granularity = TENTHS;
	}
	
	else
	{
		granularity = SECONDS;
	}
	
	if (state == State::INACTIVE)
		initTimeLabel();
	
	emit timeFormatChanged(timeFormatMsg);
}



void QStopwatch::start()
{
	if (state == State::INACTIVE)
	{
		accumulator = 0;
		elapsedTimer.start();
		
		if (timerId == INACTIVE_TIMER_ID)
		{
			timerId = startTimer(granularity);
		}
	}
	else if (state == State::PAUSED)
	{
		elapsedTimer.restart();
		timerId = startTimer(granularity);
	}
	
	state = State::RUNNING;
}

void QStopwatch::pause()
{
	if (elapsedTimer.isValid()) 
	{
		accumulator += elapsedTimer.elapsed();
	} 
	
	elapsedTimer.invalidate();
	state = State::PAUSED;
}

void QStopwatch::reset()
{
	elapsedTimer.invalidate();
	initTimeLabel();
	state = State::INACTIVE;
}

void QStopwatch::lap()
{
	
	QTime qtime(0, 0);
	
	qtime = qtime.addMSecs(accumulator);
	
	if (elapsedTimer.isValid())
	{
		qtime = qtime.addMSecs(elapsedTimer.elapsed());
	}
	
	emit lap(qtime);
}


void QStopwatch::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != timerId)  // forward undesired events
	{
		QWidget::timerEvent(event);
		return;
	}
	
	QTime qtime(0, 0);
	
	qtime = qtime.addMSecs(accumulator);
	
	if (elapsedTimer.isValid())
	{
		qtime = qtime.addMSecs(elapsedTimer.elapsed());
	}
	
	else
	{
		killTimer(timerId);
		timerId = INACTIVE_TIMER_ID;
	}
	
	updateTimeLabel(qtime);
}

void QStopwatch::initTimeLabel()
{
	QFont labelFont;
	labelFont.setPointSize(54);
	timeLabel->setFont(labelFont);
	timeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	
	QTime startTime(0, 0);
	timeLabel->setText(startTime.toString(timeFormat));
}

void QStopwatch::updateTimeLabel(const QTime& time)
{
	QString formattedTime;
	
	if (timeFormat.endsWith("zzz"))
		formattedTime = time.toString(timeFormat);
	
	else if (timeFormat.endsWith("zz"))
	{
		QString original = time.toString(timeFormat + "z");
		formattedTime = original.left(original.size() - 1);
	}
	
	else if (timeFormat.endsWith("z"))
	{
		QString original = time.toString(timeFormat + "zz");
		formattedTime = original.left(original.size() - 2);
	}
	
	else
	{
		formattedTime = time.toString(timeFormat);
	}
	
	timeLabel->setText(formattedTime);
}



