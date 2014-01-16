#include <QBoxLayout>

#include "qtimedisplay.h"
#include "utils/utils.h"

QTimeDisplay::QTimeDisplay(QWidget *parent) : QWidget(parent), displayTime(0, 0)
{
    displayLabel = new QLabel(this);
    displayLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    displayLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(displayLabel);
}

void QTimeDisplay::setTimeFormat(const QString& format)
{
    timeFormat = format;
    displayLabel->setText(utils::formatTime(timeFormat, displayTime));
}

void QTimeDisplay::setDisplayFont(const QFont& font)
{
    displayFont = font;
    displayLabel->setFont(displayFont);
}

void QTimeDisplay::time(const QTime& t)
{
    displayTime = t;
    displayLabel->setText(utils::formatTime(timeFormat, displayTime));
}

