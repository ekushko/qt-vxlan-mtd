#pragma once

#include <QDialog>
#include <QMap>

namespace Ui
{
    class VMTDDeviceScannerDialog;
}

namespace VMTDLib
{
    class VMTDDeviceScannerDialog : public QDialog
    {
        Q_OBJECT

    public:

        explicit VMTDDeviceScannerDialog(QWidget *parent = nullptr);
        ~VMTDDeviceScannerDialog();

        enum class EnType
        {
            BANNED  = 0,
            ALLOWED = 1
        };
        Q_ENUM(EnType)

        void initializeView(EnType type,
                            const QMap<int, QString> &hosts,
                            const QMap<int, QString> &hostIps);

        int     id() const;
        void setId(int id);

        int     hostId() const;

        QString hostName() const;
        void setHostName(const QString &hostName);

        int     remaining() const;
        void setRemaining(int remaining);

    private:

        Ui::VMTDDeviceScannerDialog *ui;

        int m_id;

        QMap<int, QString> m_hosts;
        QMap<int, QString> m_hostIps;

    private slots:

        void on_cbHostName_currentIndexChanged(int index);

        void on_pbAccept_clicked();
        void on_pbCancel_clicked();
    };
}
