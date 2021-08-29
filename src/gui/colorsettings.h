/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLORSETTINGS_H
#define COLORSETTINGS_H

#include "ui_colorsettings.h"

class ColorSettings : public QWidget, public Ui::ColorSettings
{
    Q_OBJECT

public:
    explicit ColorSettings(QWidget *parent = nullptr);
};

#endif
