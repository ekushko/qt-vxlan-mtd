#pragma once

#include <QDialog>

namespace Ui
{
    class VMTDDeviceSwitchDialog;
}

namespace VMTDLib
{
    class VMTDDeviceSwitchDialog : public QDialog
    {
        Q_OBJECT

    public:

        explicit VMTDDeviceSwitchDialog(QWidget *parent = nullptr);
        ~VMTDDeviceSwitchDialog();

        int     id() const;
        void setId(int id);

        QString name() const;
        void setName(const QString &name);

        QString url() const;
        void setUrl(const QString &url);

        QString username() const;
        void setUsername(const QString &username);

        QString password() const;
        void setPassword(const QString &password);

    private:

        Ui::VMTDDeviceSwitchDialog *ui;

        int m_id;

    private slots:

        void on_pbAccept_clicked();
        void on_pbCancel_clicked();
    };
}
