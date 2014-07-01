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

#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <QWidget>
#include <QTime>

class QHBoxLayout;
class QVBoxLayout;
class QFrame;
class QLabel;
class DigitDisplay;

#include "timeformat.h"

/**
 * @brief A custom widget displaying a QTime
 * This custom widget implements a "digital" display for a time, formatted according to a certain format.
 * This widget can be connected to a generic "time source" producing the time to be displayed.
 */
class TimeDisplay : public QWidget
{
    Q_OBJECT

public:

    explicit TimeDisplay(QWidget *parent = nullptr);

    /**
     * Set the internal time format of the display
     * @param format
     */
    void setTimeFormat(const TimeFormat& format);

    /**
     * Set a custom font for hours
     * @param font The custom font to set.
     */
    void setHourFont(const QFont& font);

    /**
     * Set a custom font for minutes
     * @param font The custom font to set.
     */
    void setMinFont(const QFont& font);

    /**
     * Set a custom font for seconds
     * @param font The custom font to set.
     */
    void setSecFont(const QFont& font);

    /**
     * Set a custom font for second fractions
     * @param font The custom font to set.
     */
    void setFracFont(const QFont& font);

    /**
     * Set a custom color for display background.
     * @param color The custom color to set.
     */
    void setBackgroundColor(const QColor& color);

    /**
     * Set a custom color for display fonts.
     * @param color The custom color to set.
     */
    void setTextColor(const QColor& color);

    /**
     * Show or hide the time headers of the display.
     * @param show Whether to show the time headers.
     */
    void showHeaders(bool show);

    /**
     * Get the current time formatted with the current format.
     * @return Current time formatted as string.
     */
    QString currentTime();

public slots:

    /**
     * Set the time to be displayed.
     * @param t The time to be displayed.
     */
    void time(const QTime& t);

private:

    static const int MIN_FRAME_WIDTH = 100;     /** Minimum default frame width used by all frames */

    QHBoxLayout *displayLayout;
    QVBoxLayout *hourLayout;
    QVBoxLayout *minLayout;
    QVBoxLayout *secLayout;
    QVBoxLayout *fracLayout;

    QFrame *hourFrame;
    QFrame *minFrame;
    QFrame *secFrame;
    QFrame *fracFrame;

    QLabel *hourHeader;
    QLabel *minHeader;
    QLabel *secHeader;
    QLabel *fracHeader;
    DigitDisplay *hourDisplay;
    DigitDisplay *minDisplay;
    DigitDisplay *secDisplay;
    DigitDisplay *fracDisplay;

    QColor backgroundColor;
    QColor textColor;

    QTime displayTime;              /** Current display time */
    TimeFormat timeFormat;          /** Current display time format */

    /**
     * Refresh the labels text implementing the display timer
     */
    void updateTimer();

    /**
     * Refresh the minimum width of the frames, based on current font sizes
     */
    void updateWidth();
};

#endif
