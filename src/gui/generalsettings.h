/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include "ui_generalsettings.h"

class GeneralSettings : public QWidget, public Ui::GeneralSettings
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = nullptr);

private Q_SLOTS:

    void showFractionsToggled(bool toggled);
    void showLapFractionsToggled(bool toggled);
    void enableLapsToggled(bool toggled);
};

#endif
