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

    hourLabel = new QLabel(hourFrame);
    minLabel = new QLabel(minFrame);
    secLabel = new QLabel(secFrame);
    fracLabel = new QLabel(fracFrame);

    hourLabel->setAlignment(Qt::AlignRight);
    minLabel->setAlignment(Qt::AlignRight);
    secLabel->setAlignment(Qt::AlignRight);
    fracLabel->setAlignment(Qt::AlignRight);
    hourLabel->setText(timeFormat.formatHours(displayTime));
    minLabel->setText(timeFormat.formatMin(displayTime));
    secLabel->setText(timeFormat.formatSec(displayTime));
    fracLabel->setText(timeFormat.formatSecFrac(displayTime));
    hourLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    minLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    secLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fracLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    hourLayout->addWidget(hourHeader);
    hourLayout->addWidget(hourLabel);
    minLayout->addWidget(minHeader);
    minLayout->addWidget(minLabel);
    secLayout->addWidget(secHeader);
    secLayout->addWidget(secLabel);
    fracLayout->addWidget(fracHeader);
    fracLayout->addWidget(fracLabel);

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

    if (timeFormat.isSecFracEnabled()) {
        if (timeFormat.isMSecEnabled()) {
            fracHeader->setText(i18n("Milliseconds"));
        }
        else if (timeFormat.isHundredthEnabled()) {
            fracHeader->setText(i18n("Hundredths"));
        }
        else if (timeFormat.isTenthEnabled()) {
            fracHeader->setText(i18n("Tenths"));
        }
    }

    updateTimer();
}

void QTimeDisplay::setHourFont(const QFont& font)
{
    hourFont = font;
    hourLabel->setFont(hourFont);
}

void QTimeDisplay::setMinFont(const QFont& font)
{
    minFont = font;
    minLabel->setFont(minFont);
}

void QTimeDisplay::setSecFont(const QFont& font)
{
    secFont = font;
    secLabel->setFont(secFont);
}

void QTimeDisplay::setFracFont(const QFont& font)
{
    fracFont = font;
    fracLabel->setFont(fracFont);
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
    return timeFormat.format(displayTime);
}


void QTimeDisplay::time(const QTime& t)
{
    displayTime = t;
    updateTimer();
}

void QTimeDisplay::updateTimer()
{
    if (timeFormat.isHourEnabled()) {
        hourLabel->setText(timeFormat.formatHours(displayTime));
    }

    if (timeFormat.isMinEnabled()) {
        minLabel->setText(timeFormat.formatMin(displayTime));
    }

    if (timeFormat.isSecEnabled()) {
        secLabel->setText(timeFormat.formatSec(displayTime));
    }

    if (timeFormat.isSecFracEnabled()) {
        fracLabel->setText(timeFormat.formatSecFrac(displayTime));
    }
}

