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

#include <QVBoxLayout>

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
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(timeLabel);
	
	initTimeLabel();
}

QStopwatch::~QStopwatch()
{

}

QString QStopwatch::format() const
{
	return timeFormatMsg;
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
	
	timeLabel->setText(qtime.toString(timeFormat));
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
