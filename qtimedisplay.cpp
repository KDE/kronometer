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

#include "qtimedisplay.h"

#include <KLocale>

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>

#include "qdigitdisplay.h"

namespace
{
    const QString FRAME_STYLE = "QFrame {background-clip: content; background: %1; color: %2}";
}

QTimeDisplay::QTimeDisplay(QWidget *parent) : QWidget(parent), displayTime(0, 0)
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

    hourDisplay = new QDigitDisplay(hourFrame);
    minDisplay = new QDigitDisplay(minFrame);
    secDisplay = new QDigitDisplay(secFrame);
    fracDisplay = new QDigitDisplay(fracFrame);

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

void QTimeDisplay::setTimeFormat(const QTimeFormat &format)
{
    timeFormat = format;

    hourFrame->setVisible(timeFormat.isHourEnabled());
    minFrame->setVisible(timeFormat.isMinEnabled());
    secFrame->setVisible(timeFormat.isSecEnabled());
    fracFrame->setVisible(timeFormat.isSecFracEnabled());

    hourDisplay->setDigitCounter(QDigitDisplay::ONE_DIGIT);
    minDisplay->setDigitCounter(QDigitDisplay::TWO_DIGITS);
    secDisplay->setDigitCounter(QDigitDisplay::TWO_DIGITS);

    if (timeFormat.isSecFracEnabled()) {
        if (timeFormat.isMSecEnabled()) {
            fracHeader->setText(i18n("Milliseconds"));
            fracDisplay->setDigitCounter(QDigitDisplay::THREE_DIGITS);
        }
        else if (timeFormat.isHundredthEnabled()) {
            fracHeader->setText(i18n("Hundredths"));
            fracDisplay->setDigitCounter(QDigitDisplay::TWO_DIGITS);
        }
        else if (timeFormat.isTenthEnabled()) {
            fracHeader->setText(i18n("Tenths"));
            fracDisplay->setDigitCounter(QDigitDisplay::ONE_DIGIT);
        }
    }

    updateTimer();
}

void QTimeDisplay::setHourFont(const QFont& font)
{
    hourFont = font;
    hourDisplay->setFont(hourFont);
    updateWidth();
}

void QTimeDisplay::setMinFont(const QFont& font)
{
    minFont = font;
    minDisplay->setFont(minFont);
    updateWidth();
}

void QTimeDisplay::setSecFont(const QFont& font)
{
    secFont = font;
    secDisplay->setFont(secFont);
    updateWidth();
}

void QTimeDisplay::setFracFont(const QFont& font)
{
    fracFont = font;
    fracDisplay->setFont(fracFont);
    updateWidth();
}

void QTimeDisplay::setBackgroundColor(const QColor& color)
{
    backgroundColor = color;

    hourFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    minFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    secFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    fracFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
}

void QTimeDisplay::setTextColor(const QColor& color)
{
    textColor = color;

    hourFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    minFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    secFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
    fracFrame->setStyleSheet(FRAME_STYLE.arg(backgroundColor.name(), textColor.name()));
}

void QTimeDisplay::showHeaders(bool show)
{
    hourHeader->setVisible(show);
    minHeader->setVisible(show);
    secHeader->setVisible(show);
    fracHeader->setVisible(show);
}

QString QTimeDisplay::currentTime()
{
    timeFormat.showDividers(true);
    QString currentTime = timeFormat.format(displayTime);
    timeFormat.showDividers(false);

    return currentTime;
}


void QTimeDisplay::time(const QTime& t)
{
    displayTime = t;
    updateTimer();
}

void QTimeDisplay::updateTimer()
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

void QTimeDisplay::updateWidth()
{
    int width = MIN_FRAME_WIDTH;

    int maxWidth = qMax(qMax(hourDisplay->width(), minDisplay->width()), qMax(secDisplay->width(), fracDisplay->width()));

    if (maxWidth >= MIN_FRAME_WIDTH) {
        width = maxWidth + (maxWidth * 20 / 100); // 20% as padding, i.e. 10% as right padding and 10% as left padding
    }

    hourFrame->setMinimumWidth(width);
    minFrame->setMinimumWidth(width);
    secFrame->setMinimumWidth(width);
    fracFrame->setMinimumWidth(width);
}

