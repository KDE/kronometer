/*
    Copyright (C) 2014 by Elvis Angelaccio <elvis.angelaccio@kde.org>

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

#include <QHBoxLayout>
#include <QLabel>

DigitDisplay::DigitDisplay(QWidget *parent, DigitCounter counter) : QWidget(parent)
{
    auto layout = new QHBoxLayout {this};
    m_leftmostDigit = new QLabel {this};
    m_centerDigit = new QLabel {this};
    m_rightmostDigit = new QLabel {this};

    m_leftmostDigit->setAlignment(Qt::AlignCenter);
    m_centerDigit->setAlignment(Qt::AlignCenter);
    m_rightmostDigit->setAlignment(Qt::AlignCenter);

    layout->addWidget(m_leftmostDigit);
    layout->addWidget(m_centerDigit);
    layout->addWidget(m_rightmostDigit);

    setDigitCounter(counter);
}

void DigitDisplay::setDigitCounter(DigitCounter counter)
{
    switch (counter) {
    case OneDigit:
        m_leftmostDigit->show();
        m_centerDigit->hide();
        m_rightmostDigit->hide();
        m_digitCounter = counter;
        break;
    case TwoDigits:
        m_leftmostDigit->show();
        m_centerDigit->show();
        m_rightmostDigit->hide();
        m_digitCounter = counter;
        break;
    case ThreeDigits:
        m_leftmostDigit->show();
        m_centerDigit->show();
        m_rightmostDigit->show();
        m_digitCounter = counter;
        break;
    default:
        m_leftmostDigit->hide();
        m_centerDigit->hide();
        m_rightmostDigit->hide();
        m_digitCounter = NoDigit;
        break;
    }
}

void DigitDisplay::showDigits(const QString& digits) const
{
    switch (m_digitCounter) {
    case OneDigit:
        showOneDigit(digits);
        break;
    case TwoDigits:
        showTwoDigits(digits);
        break;
    case ThreeDigits:
        showThreeDigits(digits);
        break;
    default:
        break;
    }
}

void DigitDisplay::setFont(const QFont& font)
{
    m_displayFont = font;

    m_leftmostDigit->setFont(m_displayFont);
    m_centerDigit->setFont(m_displayFont);
    m_rightmostDigit->setFont(m_displayFont);
}

QSize DigitDisplay::minimumSizeHint() const
{
    auto width = 0;
    auto fontMetrics = QFontMetrics {m_displayFont};

    switch (m_digitCounter) {
    case OneDigit:
        width = fontMetrics.width(m_leftmostDigit->text());
        break;
    case TwoDigits:
        width = fontMetrics.width(m_leftmostDigit->text()) + fontMetrics.width(m_centerDigit->text());
        break;
    case ThreeDigits:
        width = fontMetrics.width(m_leftmostDigit->text()) + fontMetrics.width(m_centerDigit->text()) + fontMetrics.width(m_rightmostDigit->text());
        break;
    default:
        break;
    }

    width += (width * 10 / 100); // 10% used as padding between digits

    return {width, QWidget::minimumSizeHint().height()};
}

void DigitDisplay::showOneDigit(const QString& digit) const
{
    if (digit.size() != m_digitCounter) {
        return;
    }

    m_leftmostDigit->setText(digit.at(0));
}

void DigitDisplay::showTwoDigits(const QString& digits) const
{
    if (digits.size() != m_digitCounter) {
        return;
    }

    // digits are displayed from right to left
    m_centerDigit->setText(digits.at(1));
    m_leftmostDigit->setText(digits.at(0));
}

void DigitDisplay::showThreeDigits(const QString& digits) const
{
    if (digits.size() != m_digitCounter) {
        return;
    }

    // digits are displayed from right to left
    m_rightmostDigit->setText(digits.at(2));
    m_centerDigit->setText(digits.at(1));
    m_leftmostDigit->setText(digits.at(0));
}