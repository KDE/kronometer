/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DIGITDISPLAY_H
#define DIGITDISPLAY_H

#include <QWidget>

class QLabel;

/**
 * @brief A custom widget displaying up to three digits.
 * This widget is meant to be used as a QLabel replacement to display the digits
 * of a timer component (minutes, seconds, etc.). By default a QLabel displays
 * a single text and if the font is not monospace this might look bad when the
 * text is refreshed very quickly, as in a stopwatch timer. This widget instead
 * displays the text by splitting it into up to three different strings, displayed
 * in different QLabels within a horizontal layout. (i.e. simulating a monospace font
 *  using adequate padding).
 */
class DigitDisplay : public QWidget
{
    Q_OBJECT

public:

    enum class Digits
    {
        One,      /**< Display one digit. */
        Two,      /**< Display two digits. */
        Three,    /**< Display three digits. */
        None      /**< Display no digit. */
    };

    explicit DigitDisplay(QWidget *parent = nullptr, Digits digits = Digits::None);

    /**
     * Set the number of digits to be displayed.
     * @param digits The number of digits to be displayed.
     */
    void setDigits(Digits digits);

    /**
     * The digits to be displayed.
     * The input string shall have a length equal to the internal digit counter.
     * Otherwise nothing is displayed.
     * @param digits The digits string to be displayed.
     */
    void showDigits(const QString& digits) const;

    /**
     * Set a custom font for the widget labels.
     * @param font The custom font to set.
     */
    void setFont(const QFont& font);

    virtual QSize minimumSizeHint() const override;

private:

    QLabel *m_leftmostDigit;
    QLabel *m_centerDigit;
    QLabel *m_rightmostDigit;

    QFont m_displayFont;
    Digits m_digits;

    /**
     * @return Whether the length of @p text is consistent with the display digits.
     */
    bool isValid(const QString& text) const;

    /**
     * Helper function to display a single digit.
     * @param digit The digit to be displayed.
     */
    void showOneDigit(const QString& digit) const;

    /**
     * Helper function to display two digits.
     * @param digits The digits to be displayed.
     */
    void showTwoDigits(const QString& digits) const;

    /**
     * Helper function to display three digits.
     * @param digits The digits to be displayed.
     */
    void showThreeDigits(const QString& digits) const;

    Q_DISABLE_COPY(DigitDisplay)
};

#endif
