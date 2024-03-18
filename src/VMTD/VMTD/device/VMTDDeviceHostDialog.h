#pragma once

#include <QDialog>

namespace Ui
{
    class VMTDDeviceHostDialog;
}

namespace VMTDLib
{
    class VMTDDeviceHostDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit VMTDDeviceHostDialog(QWidget *parent = nullptr);
        ~VMTDDeviceHostDialog();

        void initializeView(const QMap<int, QString> &switches);

        int     id() const;
        void setId(int id);

        QString name() const;
        void setName(const QString &name);

        QString ip() const;
        void setIp(const QString &ip);

        QString interface() const;
        void setInterface(const QString &interface);

        QString switchport() const;
        void setSwitchport(const QString &switchport);

        int     switchId() const;

        QString switchName() const;
        void setSwitchName(const QString &switchName);

    private:

        Ui::VMTDDeviceHostDialog *ui;

        int m_id;

    private slots:

        void on_pbAccept_clicked();
        void on_pbCancel_clicked();
    };
}
