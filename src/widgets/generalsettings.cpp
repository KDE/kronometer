/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

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
    lapTimesFormat->setVisible(toggled);
}
