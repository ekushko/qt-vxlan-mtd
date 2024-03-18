#pragma once

#include "VMTDDeviceManager.h"
#include "VMTDDeviceTableForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDDeviceManagerForm;
}

namespace VMTDLib
{
    class VMTDDeviceManagerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDDeviceManagerForm(QWidget *parent, VMTDDeviceManager *manager);
        ~VMTDDeviceManagerForm();

    private:

        Ui::VMTDDeviceManagerForm *ui;

        VMTDDeviceManager *m_manager;

        VMTDDeviceTableForm *m_switchesForm;
        VMTDDeviceTableForm    *m_hostsForm;
    };
}
