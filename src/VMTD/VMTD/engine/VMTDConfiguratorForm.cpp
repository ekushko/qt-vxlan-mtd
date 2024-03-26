#include    "VMTDConfiguratorForm.h"
#include "ui_VMTDConfiguratorForm.h"

namespace VMTDLib
{
    VMTDConfiguratorForm::VMTDConfiguratorForm(QWidget *parent, VMTDConfigurator *configurator) :
        QWidget(parent),
        ui(new Ui::VMTDConfiguratorForm),
        m_configurator(configurator)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDConfiguratorForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        updateView();
    }

    VMTDConfiguratorForm::~VMTDConfiguratorForm()
    {
        delete ui;
    }

    void VMTDConfiguratorForm::updateView()
    {
        ui->lbNetplan1->setText(m_configurator->netplan1().isEmpty()
                                ? "Empty"
                                : m_configurator->netplan1());
        ui->lbNetplan2->setText(m_configurator->netplan2().isEmpty()
                                ? "Empty"
                                : m_configurator->netplan2());
        ui->lbHosts->setText(m_configurator->hosts().isEmpty()
                             ? "Empty"
                             : m_configurator->hosts());
    }

    void VMTDConfiguratorForm::uiTimerTickSlot()
    {
        updateView();
    }
}
