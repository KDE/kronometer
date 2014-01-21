/*
    Copyright (C) 2014 by Elvis Angelaccio <angelaccioelvis@gmail.com>

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

#ifndef SAVE_SETTINGS_H
#define SAVE_SETTINGS_H

#include "ui_savesettings.h"

class SaveSettings : public QWidget, public Ui::SaveSettings
{

public:
    explicit SaveSettings(QWidget *parent) : QWidget(parent)
    {
        setupUi(this);
    }
};

#endif
