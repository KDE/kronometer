#ifndef SAVE_SETTINGS_H
#define SAVE_SETTINGS_H

#include "ui_savesettings.h"

class SaveSettings: public QWidget, public Ui::SaveSettings
{

public:
    explicit SaveSettings(QWidget *parent) : QWidget(parent)
    {
        setupUi(this);
    }
};

#endif
