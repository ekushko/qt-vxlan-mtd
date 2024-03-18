#include    "VMTDDeviceSwitchDialog.h"
#include "ui_VMTDDeviceSwitchDialog.h"

namespace VMTDLib
{
    VMTDDeviceSwitchDialog::VMTDDeviceSwitchDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::VMTDDeviceSwitchDialog),
        m_id(-1)
    {
        ui->setupUi(this);

        setWindowModality(Qt::WindowModal);

        setWindowTitle("Switch");
    }

    VMTDDeviceSwitchDialog::~VMTDDeviceSwitchDialog()
    {
        delete ui;
    }

    int VMTDDeviceSwitchDialog::id() const
    {
        return m_id;
    }
    void VMTDDeviceSwitchDialog::setId(int id)
    {
        m_id = id;
    }

    QString VMTDDeviceSwitchDialog::name() const
    {
        return ui->leName->text();
    }
    void VMTDDeviceSwitchDialog::setName(const QString &name)
    {
        ui->leName->setText(name);
    }

    QString VMTDDeviceSwitchDialog::url() const
    {
        return ui->leUrl->text();
    }
    void VMTDDeviceSwitchDialog::setUrl(const QString &url)
    {
        ui->leUrl->setText(url);
    }

    QString VMTDDeviceSwitchDialog::username() const
    {
        return ui->leUsername->text();
    }
    void VMTDDeviceSwitchDialog::setUsername(const QString &username)
    {
        ui->leUsername->setText(username);
    }

    QString VMTDDeviceSwitchDialog::password() const
    {
        return ui->lePassword->text();
    }
    void VMTDDeviceSwitchDialog::setPassword(const QString &password)
    {
        ui->lePassword->setText(password);
    }

    void VMTDDeviceSwitchDialog::on_pbAccept_clicked()
    {
        accept();
    }
    void VMTDDeviceSwitchDialog::on_pbCancel_clicked()
    {
        reject();
    }
}
