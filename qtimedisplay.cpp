#include <KLocale>

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>

#include "qtimedisplay.h"

namespace
{
    const QByteArray HOURS_MSG = "Hours";
    const QByteArray MIN_MSG = "Minutes";
    const QByteArray SEC_MSG = "Seconds";
    const QByteArray TENTHS_MSG = "Tenths";
    const QByteArray HUNDREDTHS_MSG = "Hundredths";
    const QByteArray MSEC_MSG = "Milliseconds";
}

QTimeDisplay::QTimeDisplay(QWidget *parent) : QWidget(parent), displayTime(0, 0)
{
    displayLayout = new QHBoxLayout(this);

    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    hourFrame = new QFrame(splitter);
    minFrame = new QFrame(splitter);
    secFrame = new QFrame(splitter);
    fracFrame = new QFrame(splitter);
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
    hourHeader->setText(i18n(HOURS_MSG));
    minHeader->setText(i18n(MIN_MSG));
    secHeader->setText(i18n(SEC_MSG));
    fracHeader->setText(i18n(HUNDREDTHS_MSG));
    hourHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    minHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    secHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fracHeader->setTextInteractionFlags(Qt::TextSelectableByMouse);

    hourLabel = new QLabel(hourFrame);
    minLabel = new QLabel(minFrame);
    secLabel = new QLabel(secFrame);
    fracLabel = new QLabel(fracFrame);

    hourLabel->setAlignment(Qt::AlignCenter);
    minLabel->setAlignment(Qt::AlignCenter);
    secLabel->setAlignment(Qt::AlignCenter);
    fracLabel->setAlignment(Qt::AlignCenter);
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

    splitter->addWidget(hourFrame);
    splitter->addWidget(minFrame);
    splitter->addWidget(secFrame);
    splitter->addWidget(fracFrame);

    displayLayout->addWidget(splitter);
}

void QTimeDisplay::setTimeFormat(const TimeFormat& format)
{
    timeFormat = format;

    hourFrame->setVisible(timeFormat.isHourEnabled());
    minFrame->setVisible(timeFormat.isMinEnabled());
    secFrame->setVisible(timeFormat.isSecEnabled());
    fracFrame->setVisible(timeFormat.isSecFracEnabled());

    if (timeFormat.isSecFracEnabled())
    {
        if (timeFormat.isMSecEnabled())
        {
            fracHeader->setText(i18n(MSEC_MSG));
        }

        else if (timeFormat.isHundredthEnabled())
        {
            fracHeader->setText(i18n(HUNDREDTHS_MSG));
        }

        else if (timeFormat.isTenthEnabled())
        {
            fracHeader->setText(i18n(TENTHS_MSG));
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
    if (timeFormat.isHourEnabled())
    {
        hourLabel->setText(timeFormat.formatHours(displayTime));
    }

    if (timeFormat.isMinEnabled())
    {
        minLabel->setText(timeFormat.formatMin(displayTime));
    }

    if (timeFormat.isSecEnabled())
    {
        secLabel->setText(timeFormat.formatSec(displayTime));
    }

    if (timeFormat.isSecFracEnabled())
    {
        fracLabel->setText(timeFormat.formatSecFrac(displayTime));
    }
}

