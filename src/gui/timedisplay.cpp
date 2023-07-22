/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "timedisplay.h"
#include "digitdisplay.h"

#include <KLocalizedString>

#include <QGroupBox>

TimeDisplay::TimeDisplay(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    m_hourDisplay->setDigits(DigitDisplay::Digits::Two);
    m_minDisplay->setDigits(DigitDisplay::Digits::Two);
    m_secDisplay->setDigits(DigitDisplay::Digits::Two);

    m_hourDisplay->showDigits(m_currentFormat.formatHours(m_displayTime));
    m_minDisplay->showDigits(m_currentFormat.formatMinutes(m_displayTime));
    m_secDisplay->showDigits(m_currentFormat.formatSeconds(m_displayTime));
    m_fracDisplay->showDigits(m_currentFormat.formatFractions(m_displayTime));
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
    const auto currentTime = m_currentFormat.format(m_displayTime);
    m_currentFormat.showDividers(false);

    return currentTime;
}

void TimeDisplay::setTime(int time)
{
    m_displayTime.setHMS(time / MSECS_PER_HOUR,
                        (time % MSECS_PER_HOUR) / MSECS_PER_MIN,
                        (time / MSECS_PER_SEC) % 60,
                        time % MSECS_PER_SEC);

    updateTimer();
}

void TimeDisplay::reset()
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
    auto width = qMax(qMax(m_hourDisplay->minimumSizeHint().width(),
                          m_minDisplay->minimumSizeHint().width()),
                     qMax(m_secDisplay->minimumSizeHint().width(),
                          m_fracDisplay->minimumSizeHint().width()));

    width = width + (width * 40 / 100); // 40% as padding, i.e. 20% as right padding and 20% as left padding

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
            m_fracDisplay->setDigits(DigitDisplay::Digits::One);
            break;
        case TimeFormat::UpToHundredths:
            m_fracGroup->setTitle(i18nc("@title:column", "Hundredths"));
            m_fracDisplay->setDigits(DigitDisplay::Digits::Two);
            break;
        case TimeFormat::UpToMilliseconds:
            m_fracGroup->setTitle(i18nc("@title:column", "Milliseconds"));
            m_fracDisplay->setDigits(DigitDisplay::Digits::Three);
            break;
        default:
            break;
        }
    }

    updateTimer();
}

void TimeDisplay::setGroupboxColor(QGroupBox *groupBox, QPalette::ColorRole role, const QColor& color)
{
    if (!groupBox) {
        return;
    }

    auto palette = QPalette {groupBox->palette()};
    palette.setColor(role, color);
    groupBox->setPalette(palette);
}


#include "moc_timedisplay.cpp"
