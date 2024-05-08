#include    "VMTDDeviceHostDialog.h"
#include "ui_VMTDDeviceHostDialog.h"

#include "../VMTDComboMap.h"

namespace VMTDLib
{
    VMTDDeviceHostDialog::VMTDDeviceHostDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::VMTDDeviceHostDialog),
        m_id(-1)
    {
        ui->setupUi(this);

        setWindowModality(Qt::WindowModal);

        setWindowTitle("Host");
    }

    VMTDDeviceHostDialog::~VMTDDeviceHostDialog()
    {
        delete ui;
    }

    void VMTDDeviceHostDialog::initializeView(const QMap<int, QString> &switches)
    {
        ui->cbSwitchName->clear();

        CB_FILL_MAP(ui->cbSwitchName, switches);
    }

    int VMTDDeviceHostDialog::id() const
    {
        return m_id;
    }
    void VMTDDeviceHostDialog::setId(int id)
    {
        m_id = id;
    }

    QString VMTDDeviceHostDialog::name() const
    {
        return ui->leName->text();
    }
    void VMTDDeviceHostDialog::setName(const QString &name)
    {
        ui->leName->setText(name);
    }

    QString VMTDDeviceHostDialog::domainName() const
    {
        return ui->leDomainName->text();
    }
    void VMTDDeviceHostDialog::setDomainName(const QString &domainName)
    {
        ui->leDomainName->setText(domainName);
    }

    QString VMTDDeviceHostDialog::ip() const
    {
        return ui->leIp->text();
    }
    void VMTDDeviceHostDialog::setIp(const QString &ip)
    {
        ui->leIp->setText(ip);
    }

    QString VMTDDeviceHostDialog::interface() const
    {
        return ui->leInterface->text();
    }
    void VMTDDeviceHostDialog::setInterface(const QString &interface)
    {
        ui->leInterface->setText(interface);
    }

    QString VMTDDeviceHostDialog::switchport() const
    {
        return ui->leSwitchport->text();
    }
    void VMTDDeviceHostDialog::setSwitchport(const QString &switchport)
    {
        ui->leSwitchport->setText(switchport);
    }

    int VMTDDeviceHostDialog::switchId() const
    {
        return CB_DATA_TOI(ui->cbSwitchName);
    }

    QString VMTDDeviceHostDialog::switchName() const
    {
        return ui->cbSwitchName->currentText();
    }
    void VMTDDeviceHostDialog::setSwitchName(const QString &switchName)
    {
        CB_SET_INDEX(ui->cbSwitchName, switchName);
    }

    void VMTDDeviceHostDialog::on_pbAccept_clicked()
    {
        accept();
    }
    void VMTDDeviceHostDialog::on_pbCancel_clicked()
    {
        reject();
    }
}
