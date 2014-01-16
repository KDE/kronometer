#ifndef Q_TIME_DISPLAY_H
#define Q_TIME_DISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QTime>

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
    void setTimeFormat(const QString& format);

   /**
    * Set a custom font for time display
    * @param font The custom font to set.
    */
    void setDisplayFont(const QFont& font);

public slots:

    /**
     * Set the time to be displayed.
     * @param t The time to be displayed.
     */
    void time(const QTime& t);

private:

    QLabel *displayLabel;      /** Label implementing the digital display with the timer */
    QFont displayFont;         /** Current display font */
    QTime displayTime;         /** Current display time */
    QString timeFormat;        /** Current display time format */
};

#endif
