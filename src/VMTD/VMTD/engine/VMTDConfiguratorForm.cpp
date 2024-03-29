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

        setEditMode(false);
        updateView();
    }

    VMTDConfiguratorForm::~VMTDConfiguratorForm()
    {
        delete ui;
    }

    void VMTDConfiguratorForm::setEditMode(bool isEditMode)
    {
        ui->w->setEnabled(isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDConfiguratorForm::updateView()
    {
        ui->pteNetplan1->setPlainText(m_configurator->netplan1());
        ui->pteNetplan2->setPlainText(m_configurator->netplan2());
        ui->pteHosts->setPlainText(m_configurator->hosts());
    }

    void VMTDConfiguratorForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDConfiguratorForm::on_pbChange_clicked()
    {
        setEditMode(true);
        updateView();

        m_uiTimer.stop();
    }
    void VMTDConfiguratorForm::on_pbAccept_clicked()
    {
        m_configurator->setNetplan1(ui->pteNetplan1->toPlainText());
        m_configurator->setNetplan2(ui->pteNetplan2->toPlainText());
        m_configurator->setHosts(ui->pteHosts->toPlainText());

        emit m_configurator->applyNetplanSignal();

        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
    void VMTDConfiguratorForm::on_pbCancel_clicked()
    {
        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
}
