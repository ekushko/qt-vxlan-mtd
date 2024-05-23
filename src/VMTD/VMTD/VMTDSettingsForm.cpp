#include    "VMTDSettingsForm.h"
#include "ui_VMTDSettingsForm.h"

#include "VMTDVarname.h"
#include "VMTDComboMap.h"

namespace VMTDLib
{
    VMTDSettingsForm::VMTDSettingsForm(QWidget *parent, VMTDSettings *settings) :
        QWidget(parent),
        ui(new Ui::VMTDSettingsForm),
        m_settings(settings)
    {
        ui->setupUi(this);

        setWindowTitle(QString("Settings %1").arg(m_settings->systemName()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        CB_FILL_MAP(ui->cbNodeType, VMTDSettings::enNodeTypeToL());

        setEditMode(false);
        updateView();
    }

    VMTDSettingsForm::~VMTDSettingsForm()
    {
        delete ui;
    }

    void VMTDSettingsForm::setEditMode(bool isEditMode)
    {
        ui->w->setEnabled(isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDSettingsForm::updateView()
    {
        CB_SELECT(ui->cbNodeType, m_settings->nodeType());
        ui->lbSystemName->setText(m_settings->systemName());
        ui->leDebugName->setText(m_settings->debugName());
        ui->chbShouldShowDebug->setChecked(m_settings->shouldShowDebug());

        ui->leDbName->setText(m_settings->dbName());
        ui->chbShouldCheckOnline->setChecked(m_settings->shouldCheckOnline());
        ui->chbShouldUseReconfigTimer->setChecked(m_settings->shouldUseReconfigTimer());
        ui->sbReconfigurationInterval->setValue(m_settings->reconfigInterval());
        ui->chbShouldRandomizeVlan->setChecked(m_settings->shouldRandomizeVlan());
        ui->sbDefaultVlanId->setValue(m_settings->defaultVlanId());
        ui->sbMinVlanId->setValue(m_settings->minVlanId());
        ui->sbMaxVlanId->setValue(m_settings->maxVlanId());
        ui->chbShouldRandomizeNetwork->setChecked(m_settings->shouldRandomizeNetwork());
        ui->chbShouldRandomizeGateway->setChecked(m_settings->shouldRandomizeGateway());
        ui->chbShouldRandomizeParticipant->setChecked(m_settings->shouldRandomizeParticipant());
        ui->leNetplan1FileName->setText(m_settings->netplan1FileName());
        ui->leNetplan2FileName->setText(m_settings->netplan2FileName());

        ui->sbLocalPort->setValue(m_settings->localPort());
        ui->leServerIp->setText(m_settings->serverIp());
        ui->sbServerPort->setValue(m_settings->serverPort());
        ui->chbShouldReconnect->setChecked(m_settings->shouldReconnect());
        ui->sbReconnectInterval->setValue(m_settings->reconnectInterval());

        ui->chbShouldCheckConnection->setChecked(m_settings->shouldCheckConnection());
        ui->sbCheckConnectionInterval->setValue(m_settings->checkConnectionInterval());
        ui->sbCheckQueueInterval->setValue(m_settings->checkQueueInterval());
        ui->sbTicketTimeoutInterval->setValue(m_settings->ticketTimeoutInterval());
    }

    void VMTDSettingsForm::updateData()
    {
        m_settings->setNodeType(CB_DATA_TOE(ui->cbNodeType, VMTDNodeType));
        m_settings->setDebugName(ui->leDebugName->text());
        m_settings->setShouldShowDebug(ui->chbShouldShowDebug->isChecked());

        m_settings->setDbName(ui->leDbName->text());
        m_settings->setShouldCheckOnline(ui->chbShouldCheckOnline->isChecked());
        m_settings->setShouldUseReconfigTimer(ui->chbShouldUseReconfigTimer->isChecked());
        m_settings->setReconfigInterval(ui->sbReconfigurationInterval->value());
        m_settings->setShouldRandomizeVlan(ui->chbShouldRandomizeVlan->isChecked());
        m_settings->setDefaultVlanId(ui->sbDefaultVlanId->value());
        m_settings->setMinVlanId(ui->sbMinVlanId->value());
        m_settings->setMaxVlanId(ui->sbMaxVlanId->value());
        m_settings->setShouldRandomizeNetwork(ui->chbShouldRandomizeNetwork->isChecked());
        m_settings->setShouldRandomizeGateway(ui->chbShouldRandomizeGateway->isChecked());
        m_settings->setShouldRandomizeParticipant(ui->chbShouldRandomizeParticipant->isChecked());
        m_settings->setNetplan1FileName(ui->leNetplan1FileName->text());
        m_settings->setNetplan2FileName(ui->leNetplan2FileName->text());

        m_settings->setLocalPort(ui->sbLocalPort->value());
        m_settings->setServerIp(ui->leServerIp->text());
        m_settings->setServerPort(ui->sbServerPort->value());
        m_settings->setShouldReconnect(ui->chbShouldReconnect->isChecked());
        m_settings->setReconnectInterval(ui->sbReconnectInterval->value());

        m_settings->setShouldCheckConnection(ui->chbShouldCheckConnection->isChecked());
        m_settings->setCheckConnectionInterval(ui->sbCheckConnectionInterval->value());
        m_settings->setCheckQueueInterval(ui->sbCheckQueueInterval->value());
        m_settings->setTicketTimeoutInterval(ui->sbTicketTimeoutInterval->value());
    }

    void VMTDSettingsForm::on_pbSave_clicked()
    {
        m_settings->save();
    }
    void VMTDSettingsForm::on_pbLoad_clicked()
    {
        m_settings->load();
        m_settings->apply();
    }

    void VMTDSettingsForm::on_pbChange_clicked()
    {
        setEditMode(true);
    }
    void VMTDSettingsForm::on_pbAccept_clicked()
    {
        updateData();

        m_settings->apply();
        m_settings->save();

        setEditMode(false);
        updateView();
    }
    void VMTDSettingsForm::on_pbCancel_clicked()
    {
        setEditMode(false);
        updateView();
    }
}


