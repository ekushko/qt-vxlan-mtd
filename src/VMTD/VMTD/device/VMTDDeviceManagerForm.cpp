#include    "VMTDDeviceManagerForm.h"
#include "ui_VMTDDeviceManagerForm.h"

namespace VMTDLib
{
    VMTDDeviceManagerForm::VMTDDeviceManagerForm(QWidget *parent, VMTDDeviceManager *manager) :
        QWidget(parent),
        ui(new Ui::VMTDDeviceManagerForm),
        m_manager(manager)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        m_switchesForm = new VMTDDeviceTableForm(ui->wSwitches, m_manager,
                                                 VMTDTableType::SWITCHES);
        m_hostsForm = new VMTDDeviceTableForm(ui->wHosts, m_manager,
                                              VMTDTableType::HOSTS);
        m_bannedScannersForm = new VMTDDeviceTableForm(ui->wBannedScanners, m_manager,
                                                       VMTDTableType::BANNED_SCANNERS);
        m_allowedScannersForm = new VMTDDeviceTableForm(ui->wAllowedScanners, m_manager,
                                                        VMTDTableType::ALLOWED_SCANNERS);
    }

    VMTDDeviceManagerForm::~VMTDDeviceManagerForm()
    {
        delete ui;
    }
}
