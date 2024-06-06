#include    "VMTDDeviceScannerDialog.h"
#include "ui_VMTDDeviceScannerDialog.h"

#include "../VMTDComboMap.h"


namespace VMTDLib
{
    VMTDDeviceScannerDialog::VMTDDeviceScannerDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::VMTDDeviceScannerDialog)
    {
        ui->setupUi(this);
    }

    VMTDDeviceScannerDialog::~VMTDDeviceScannerDialog()
    {
        delete ui;
    }

    void VMTDDeviceScannerDialog::initializeView(EnType type,
                                                 const QMap<int, QString> &hosts,
                                                 const QMap<int, QString> &hostIps)
    {
        ui->sbRemaining->setEnabled(type == EnType::BANNED);

        ui->cbHostName->clear();
        CB_FILL_MAP(ui->cbHostName, hosts);

        m_hosts = hosts;
        m_hostIps = hostIps;
    }

    int VMTDDeviceScannerDialog::id() const
    {
        return m_id;
    }
    void VMTDDeviceScannerDialog::setId(int id)
    {
        m_id = id;
    }

    int VMTDDeviceScannerDialog::hostId() const
    {
        return CB_DATA_TOI(ui->cbHostName);
    }

    QString VMTDDeviceScannerDialog::hostName() const
    {
        return ui->cbHostName->currentText();
    }
    void VMTDDeviceScannerDialog::setHostName(const QString &hostName)
    {
        CB_SET_INDEX(ui->cbHostName, hostName);
    }

    int VMTDDeviceScannerDialog::remaining() const
    {
        return ui->sbRemaining->value();
    }
    void VMTDDeviceScannerDialog::setRemaining(int remaining)
    {
        ui->sbRemaining->setValue(remaining);
    }

    void VMTDDeviceScannerDialog::on_cbHostName_currentIndexChanged(int index)
    {
        ui->lbIp->setText(m_hostIps.value(index));
    }

    void VMTDDeviceScannerDialog::on_pbAccept_clicked()
    {
        accept();
    }
    void VMTDDeviceScannerDialog::on_pbCancel_clicked()
    {
        reject();
    }
}
