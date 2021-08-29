/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generalsettings.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>

GeneralSettings::GeneralSettings(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(kcfg_showSecondFractions, &QCheckBox::toggled, this, &GeneralSettings::showFractionsToggled);
    connect(kcfg_showLapSecondFractions, &QCheckBox::toggled, this, &GeneralSettings::showLapFractionsToggled);
    connect(kcfg_isLapsRecordingEnabled, &QCheckBox::toggled, this, &GeneralSettings::enableLapsToggled);
}

void GeneralSettings::showFractionsToggled(bool toggled)
{
    kcfg_fractionsType->setEnabled(toggled);
}

void GeneralSettings::showLapFractionsToggled(bool toggled)
{
    kcfg_lapFractionsType->setEnabled(toggled);
}

void GeneralSettings::enableLapsToggled(bool toggled)
{
    lapSettings->setVisible(toggled);
}
