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

#include "digitdisplay.h"
#include "timedisplay.h"

#include <KLocalizedString>

#include <QBoxLayout>
#include <QLabel>

namespace
{
    const QString FRAME_STYLE = "QFrame {background-clip: content; background: %1; color: %2}";
}

TimeDisplay::TimeDisplay(QWidget *parent) : QWidget(parent), displayTime(0, 0)
{
    displayLayout = new QHBoxLayout(this);

    hourFrame = new QFrame(this);
    minFrame = new QFrame(this);
    secFrame = new QFrame(this);
    fracFrame = new QFrame(this);
    hourFrame->setFrameShape(QFrame::StyledPanel);
    hourFrame->setFrameShadow(QFrame::Sunken);
    minFrame->setFrameShape(QFrame::StyledPanel);
    minFrame->setFrameShadow(QFrame::Sunken);
    secFrame->setFrameShape(QFrame::StyledPanel);
    secFrame->setFrameShadow(QFrame::Sunken);
    fracFrame->setFrameShape(QFrame::StyledPanel);
    fracFrame->setFrameShadow(QFrame::Sunken);

    hourLayout = new QVBoxLayout(hourFrame);
    minLayout = new QVBoxLayout(minFrame);
    secLayout = new QVBoxLayout(secFrame);
    fracLayout = new QVBoxLayout(fracFrame);

    hourHeader = new QLabel(hourFrame);
    minHeader = new QLabel(minFrame);
    secHeader = new QLabel(secFrame);
    fracHeader = new QLabel(fracFrame);
    hourHeader->setAlignment(Qt::AlignCenter);
    minHeader->setAlignment(Qt::AlignCenter);
    secHeader->setAlignment(Qt::AlignCenter);
    fracHeader->setAlignment(Qt::AlignCenter);
    hourHeader->setText(i18n("Hours"));
    minHeader->setText(i18n("Minutes"));
    secHeader->setText(i18n("Seconds"));
    fracHeader->setText(i18n("Hundredths"));
    hourHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    minHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    secHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fracHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);

    hourDisplay = new DigitDisplay(hourFrame);
    minDisplay = new DigitDisplay(minFrame);
    secDisplay = new DigitDisplay(secFrame);
    fracDisplay = new DigitDisplay(fracFrame);

    hourDisplay->showDigits(timeFormat.formatHours(displayTime));
    minDisplay->showDigits(timeFormat.formatMin(displayTime));
    secDisplay->showDigits(timeFormat.formatSec(displayTime));
    fracDisplay->showDigits(timeFormat.formatSecFrac(displayTime));

    hourLayout->addWidget(hourHeader);
    hourLayout->addWidget(hourDisplay);
    minLayout->addWidget(minHeader);
    minLayout->addWidget(minDisplay);
    secLayout->addWidget(secHeader);
    secLayout->addWidget(secDisplay);
    fracLayout->addWidget(fracHeader);
    fracLayout->addWidget(fracDisplay);

    displayLayout->addWidget(hourFrame);
    displayLayout->addWidget(minFrame);
    displayLayout->addWidget(secFrame);
    displayLayout->addWidget(fracFrame);
}

void TimeDisplay::setTimeFormat(const TimeFormat &format)
{
    timeFormat = format;

    hourFrame->setVisible(timeFormat.isHourEnabled());
    minFrame->setVisible(timeFormat.isMinEnabled());
    secFrame->setVisible(timeFormat.isSecEnabled());
    fracFrame->setVisible(timeFormat.isSecFracEnabled());

    hourDisplay->setDigitCounter(DigitDisplay::TWO_DIGITS);
    minDisplay->setDigitCounter(DigitDisplay::TWO_DIGITS);
    secDisplay->setDigitCounter(DigitDisplay::TWO_DIGITS);

    if (timeFormat.isSecFracEnabled()) {
        if (timeFormat.isMSecEnabled()) {
            fracHeader->setText(i18n("Milliseconds"));
            fracDisplay->setDigitCounter(DigitDisplay::THREE_DIGITS);
        }
        else if (timeFormat.isHundredthEnabled()) {
            fracHeader->setText(i18n("Hundredths"));
            fracDisplay->setDigitCounter(DigitDisplay::TWO_DIGITS);
        }
        else if (timeFormat.isTenthEnabled()) {
            fracHeader->setText(i18n("Tenths"));
            fracDisplay->setDigitCounter(DigitDisplay::ONE_DIGIT);
        }
    }

    updateTimer();
}

void TimeDisplay::setHourFont(const QFont& font)
{
    hourDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setMinFont(const QFont& font)
{
    minDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setSecFont(const QFont& font)
{
    secDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setFracFont(const QFont& font)
{
    fracDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setBackgroundColor(const QColor& color)
{
    backgroundColor = color;

    hourFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    minFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    secFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    fracFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
}

void TimeDisplay::setTextColor(const QColor& color)
{
    textColor = color;

    hourFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    minFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    secFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    fracFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
}

void TimeDisplay::showHeaders(bool show)
{
    hourHeader->setVisible(show);
    minHeader->setVisible(show);
    secHeader->setVisible(show);
    fracHeader->setVisible(show);
}

QString TimeDisplay::currentTime()
{
    timeFormat.showDividers(true);
    QString currentTime = timeFormat.format(displayTime);
    timeFormat.showDividers(false);

    return currentTime;
}


void TimeDisplay::onTime(const QTime& t)
{
    displayTime = t;
    updateTimer();
}

void TimeDisplay::updateTimer()
{
    if (timeFormat.isHourEnabled()) {
        hourDisplay->showDigits(timeFormat.formatHours(displayTime));
    }

    if (timeFormat.isMinEnabled()) {
        minDisplay->showDigits(timeFormat.formatMin(displayTime));
    }

    if (timeFormat.isSecEnabled()) {
        secDisplay->showDigits(timeFormat.formatSec(displayTime));
    }

    if (timeFormat.isSecFracEnabled()) {
        fracDisplay->showDigits(timeFormat.formatSecFrac(displayTime));
    }
}

void TimeDisplay::updateWidth()
{
    int width = qMax(qMax(hourDisplay->width(), minDisplay->width()), qMax(secDisplay->width(), fracDisplay->width()));

    width = width + (width * 20 / 100); // 20% as padding, i.e. 10% as right padding and 10% as left padding

    hourFrame->setMinimumWidth(width);
    minFrame->setMinimumWidth(width);
    secFrame->setMinimumWidth(width);
    fracFrame->setMinimumWidth(width);
}

