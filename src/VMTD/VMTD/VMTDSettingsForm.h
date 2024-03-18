#pragma once

#include "VMTDSettings.h"

#include <QWidget>

namespace Ui
{
    class VMTDSettingsForm;
}

namespace VMTDLib
{
    class VMTDSettingsForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDSettingsForm(QWidget *parent, VMTDSettings *settings);
        ~VMTDSettingsForm();

    private:
        Ui::VMTDSettingsForm *ui;
    };
}
