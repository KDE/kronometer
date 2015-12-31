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

#include "timedisplay.h"
#include "digitdisplay.h"

#include <KLocalizedString>

#include <QBoxLayout>
#include <QGroupBox>

TimeDisplay::TimeDisplay(QWidget *parent) : QWidget(parent),
    m_displayTime {0, 0}
{
    auto displayLayout = new QHBoxLayout(this);

    m_hourGroup = new QGroupBox(i18nc("@title:column", "Hours"), this);
    m_minGroup = new QGroupBox(i18nc("@title:column", "Minutes"), this);
    m_secGroup = new QGroupBox(i18nc("@title:column", "Seconds"), this);
    m_fracGroup = new QGroupBox(i18nc("@title:column", "Hundredths"), this);

    auto hourLayout = new QVBoxLayout(m_hourGroup);
    auto minLayout = new QVBoxLayout(m_minGroup);
    auto secLayout = new QVBoxLayout(m_secGroup);
    auto fracLayout = new QVBoxLayout(m_fracGroup);

    m_hourDisplay = new DigitDisplay(m_hourGroup);
    m_minDisplay = new DigitDisplay(m_minGroup);
    m_secDisplay = new DigitDisplay(m_secGroup);
    m_fracDisplay = new DigitDisplay(m_fracGroup);

    m_hourDisplay->setDigitCounter(DigitDisplay::TwoDigits);
    m_minDisplay->setDigitCounter(DigitDisplay::TwoDigits);
    m_secDisplay->setDigitCounter(DigitDisplay::TwoDigits);

    m_hourDisplay->showDigits(m_currentFormat.formatHours(m_displayTime));
    m_minDisplay->showDigits(m_currentFormat.formatMinutes(m_displayTime));
    m_secDisplay->showDigits(m_currentFormat.formatSeconds(m_displayTime));
    m_fracDisplay->showDigits(m_currentFormat.formatFractions(m_displayTime));

    hourLayout->addWidget(m_hourDisplay);
    minLayout->addWidget(m_minDisplay);
    secLayout->addWidget(m_secDisplay);
    fracLayout->addWidget(m_fracDisplay);

    auto margins = displayLayout->contentsMargins();
    margins.setTop(0);
    margins.setBottom(0);

    displayLayout->setContentsMargins(margins);
    displayLayout->addWidget(m_hourGroup);
    displayLayout->addWidget(m_minGroup);
    displayLayout->addWidget(m_secGroup);
    displayLayout->addWidget(m_fracGroup);
}

void TimeDisplay::setTimeFormat(const TimeFormat& format)
{
    m_defaultFormat = format;
    m_currentFormat = format;
    updateTimeFormat();
}

void TimeDisplay::setHoursFont(const QFont& font)
{
    m_hourDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setMinutesFont(const QFont& font)
{
    m_minDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setSecondsFont(const QFont& font)
{
    m_secDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setFractionsFont(const QFont& font)
{
    m_fracDisplay->setFont(font);
    updateWidth();
}

void TimeDisplay::setBackgroundColor(const QColor& color)
{
    setGroupboxColor(m_hourGroup, m_hourGroup->backgroundRole(), color);
    setGroupboxColor(m_minGroup, m_minGroup->backgroundRole(), color);
    setGroupboxColor(m_secGroup, m_secGroup->backgroundRole(), color);
    setGroupboxColor(m_fracGroup, m_fracGroup->backgroundRole(), color);
    update();
}

void TimeDisplay::setTextColor(const QColor& color)
{
    setGroupboxColor(m_hourGroup, m_hourGroup->foregroundRole(), color);
    setGroupboxColor(m_minGroup, m_minGroup->foregroundRole(), color);
    setGroupboxColor(m_secGroup, m_secGroup->foregroundRole(), color);
    setGroupboxColor(m_fracGroup, m_fracGroup->foregroundRole(), color);
    update();
}

QString TimeDisplay::currentTime()
{
    m_currentFormat.showDividers(true);
    auto currentTime = m_currentFormat.format(m_displayTime);
    m_currentFormat.showDividers(false);

    return currentTime;
}

void TimeDisplay::slotTime(int time)
{
    m_displayTime.setHMS(time / MSECS_PER_HOUR,
                        (time % MSECS_PER_HOUR) / MSECS_PER_MIN,
                        (time / MSECS_PER_SEC) % 60,
                        time % MSECS_PER_SEC);

    updateTimer();
}

void TimeDisplay::slotReset()
{
    if (m_currentFormat == m_defaultFormat)
        return;

    setTimeFormat(m_defaultFormat);
}

void TimeDisplay::updateTimer()
{
    if (m_currentFormat.hasHours()) {
        m_hourDisplay->showDigits(m_currentFormat.formatHours(m_displayTime));
    }
    else if (m_displayTime.hour() >= 1) {
        m_currentFormat.overrideHours();
        updateTimeFormat();
    }

    if (m_currentFormat.hasMinutes()) {
        m_minDisplay->showDigits(m_currentFormat.formatMinutes(m_displayTime));
    }
    else if (m_displayTime.minute() >= 1) {
        m_currentFormat.overrideMinutes();
        updateTimeFormat();
    }

    m_secDisplay->showDigits(m_currentFormat.formatSeconds(m_displayTime));

    if (m_currentFormat.hasFractions()) {
        m_fracDisplay->showDigits(m_currentFormat.formatFractions(m_displayTime));
    }
}

void TimeDisplay::updateWidth()
{
    int width = qMax(qMax(m_hourDisplay->minimumSizeHint().width(),
                          m_minDisplay->minimumSizeHint().width()),
                     qMax(m_secDisplay->minimumSizeHint().width(),
                          m_fracDisplay->minimumSizeHint().width()));

    width = width + (width * 20 / 100); // 20% as padding, i.e. 10% as right padding and 10% as left padding

    m_hourGroup->setMinimumWidth(width);
    m_minGroup->setMinimumWidth(width);
    m_secGroup->setMinimumWidth(width);
    m_fracGroup->setMinimumWidth(width);
}

void TimeDisplay::updateTimeFormat()
{
    m_hourGroup->setVisible(m_currentFormat.hasHours());
    m_minGroup->setVisible(m_currentFormat.hasMinutes());
    m_fracGroup->setVisible(m_currentFormat.hasFractions());

    if (m_currentFormat.hasFractions()) {
        switch (m_currentFormat.secondFractions()) {
        case TimeFormat::UpToTenths:
            m_fracGroup->setTitle(i18nc("@title:column", "Tenths"));
            m_fracDisplay->setDigitCounter(DigitDisplay::OneDigit);
            break;
        case TimeFormat::UpToHundredths:
            m_fracGroup->setTitle(i18nc("@title:column", "Hundredths"));
            m_fracDisplay->setDigitCounter(DigitDisplay::TwoDigits);
            break;
        case TimeFormat::UpToMilliseconds:
            m_fracGroup->setTitle(i18nc("@title:column", "Milliseconds"));
            m_fracDisplay->setDigitCounter(DigitDisplay::ThreeDigits);
            break;
        default:
            break;
        }
    }

    updateTimer();
}

void TimeDisplay::setGroupboxColor(QGroupBox *groupBox, QPalette::ColorRole role, const QColor& color)
{
    Q_ASSERT(groupBox);
    QPalette palette = groupBox->palette();
    palette.setColor(role, color);
    groupBox->setPalette(palette);
}

