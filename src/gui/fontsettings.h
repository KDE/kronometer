/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FONTSETTINGS_H
#define FONTSETTINGS_H

#include "ui_fontsettings.h"

class FontSettings : public QWidget, public Ui::FontSettings
{
    Q_OBJECT

public:
    explicit FontSettings(QWidget *parent = nullptr);
};

#endif
