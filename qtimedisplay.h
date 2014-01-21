#ifndef Q_TIME_DISPLAY_H
#define Q_TIME_DISPLAY_H

#include <QWidget>
#include <QTime>

class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;
class QLabel;

#include "timeformat.h"

/**
 * @brief A custom widget displaying a QTime
 * This custom widget implements a "digital" display for a time, formatted according to a certain format.
 * This widget can be connected to a generic "time source" producing the time to be displayed.
 */
class QTimeDisplay : public QWidget
{
    Q_OBJECT

public:

    explicit QTimeDisplay(QWidget *parent = nullptr);

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

public slots:

    /**
     * Set the time to be displayed.
     * @param t The time to be displayed.
     */
    void time(const QTime& t);

private:

    QSplitter *splitter;
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
    QLabel *hourLabel;
    QLabel *minLabel;
    QLabel *secLabel;
    QLabel *fracLabel;

    QFont hourFont;
    QFont minFont;
    QFont secFont;
    QFont fracFont;

    QTime displayTime;              /** Current display time */
    TimeFormat timeFormat;          /** Current display time format */

    /**
     * Refresh the labels text implementing the display timer
     */
    void updateTimer();
};

#endif
