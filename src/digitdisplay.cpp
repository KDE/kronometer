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

#include <QHBoxLayout>
#include <QLabel>

DigitDisplay::DigitDisplay(QWidget *parent, DigitCounter counter) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    leftmostDigit = new QLabel(this);
    centerDigit = new QLabel(this);
    rightmostDigit = new QLabel(this);

    leftmostDigit->setAlignment(Qt::AlignCenter);
    centerDigit->setAlignment(Qt::AlignCenter);
    rightmostDigit->setAlignment(Qt::AlignCenter);

    layout->addWidget(leftmostDigit);
    layout->addWidget(centerDigit);
    layout->addWidget(rightmostDigit);

    setDigitCounter(counter);
}

void DigitDisplay::setDigitCounter(DigitCounter counter)
{
    switch (counter) {
    case ONE_DIGIT:
        leftmostDigit->show();
        centerDigit->hide();
        rightmostDigit->hide();
        digitCounter = counter;
        break;
    case TWO_DIGITS:
        leftmostDigit->show();
        centerDigit->show();
        rightmostDigit->hide();
        digitCounter = counter;
        break;
    case THREE_DIGITS:
        leftmostDigit->show();
        centerDigit->show();
        rightmostDigit->show();
        digitCounter = counter;
        break;
    default:
        leftmostDigit->hide();
        centerDigit->hide();
        rightmostDigit->hide();
        digitCounter = NO_DIGIT;
        break;
    }
}

void DigitDisplay::showDigits(const QString& digits) const
{
    switch (digitCounter) {
    case ONE_DIGIT:
        showOneDigit(digits);
        break;
    case TWO_DIGITS:
        showTwoDigits(digits);
        break;
    case THREE_DIGITS:
        showThreeDigits(digits);
        break;
    default:
        break;
    }
}

void DigitDisplay::setFont(const QFont& font)
{
    displayFont = font;

    leftmostDigit->setFont(displayFont);
    centerDigit->setFont(displayFont);
    rightmostDigit->setFont(displayFont);
}

int DigitDisplay::width() const
{
    int width = 0;
    QFontMetrics fontMetrics(displayFont);

    switch (digitCounter) {
    case ONE_DIGIT:
        width = fontMetrics.width(leftmostDigit->text());
        break;
    case TWO_DIGITS:
        width = fontMetrics.width(leftmostDigit->text()) + fontMetrics.width(centerDigit->text());
        break;
    case THREE_DIGITS:
        width = fontMetrics.width(leftmostDigit->text()) + fontMetrics.width(centerDigit->text()) + fontMetrics.width(rightmostDigit->text());
        break;
    default:
        break;
    }

    return width + (width * 10 / 100); // 10% used as padding between digits
}

void DigitDisplay::showOneDigit(const QString& digit) const
{
    if (digit.size() == digitCounter) {
        leftmostDigit->setText(digit.at(0));
    }
}

void DigitDisplay::showTwoDigits(const QString& digits) const
{
    if (digits.size() == digitCounter) {
        // digits are displayed from right to left
        centerDigit->setText(digits.at(1));
        leftmostDigit->setText(digits.at(0));
    }
}

void DigitDisplay::showThreeDigits(const QString& digits) const
{
    if (digits.size() == digitCounter) {
        // digits are displayed from right to left
        rightmostDigit->setText(digits.at(2));
        centerDigit->setText(digits.at(1));
        leftmostDigit->setText(digits.at(0));
    }
}
