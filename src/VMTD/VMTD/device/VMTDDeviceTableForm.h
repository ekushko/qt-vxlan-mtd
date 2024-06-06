#pragma once

#include "VMTDDeviceManager.h"
#include "VMTDDeviceSwitchDialog.h"
#include "VMTDDeviceHostDialog.h"
#include "VMTDDeviceScannerDialog.h"

#include <QWidget>

namespace Ui
{
    class VMTDDeviceTableForm;
}

namespace VMTDLib
{
#define VMTDTableType VMTDDeviceTableForm::EnType

    class VMTDDeviceTableForm : public QWidget
    {
        Q_OBJECT

    public:

        enum class EnType
        {
            SWITCHES,
            HOSTS,
            BANNED_SCANNERS,
            ALLOWED_SCANNERS
        };

        VMTDDeviceTableForm(QWidget *parent, VMTDDeviceManager *manager, EnType type);
        ~VMTDDeviceTableForm();

    private:

        Ui::VMTDDeviceTableForm *ui;

        VMTDDeviceManager *m_manager;

        EnType m_type;

        QSqlQueryModel *m_viewModel;

        VMTDDeviceSwitchDialog *m_switchDialog;
        VMTDDeviceHostDialog *m_hostDialog;
        VMTDDeviceScannerDialog *m_bannedScannerDialog;
        VMTDDeviceScannerDialog *m_allowedScannerDialog;

    private slots:

        void        switchesUpdatedSlot(const QSqlQuery &query);
        void           hostsUpdatedSlot(const QSqlQuery &query);
        void  bannedScannersUpdatedSlot(const QSqlQuery &query);
        void allowedScannersUpdatedSlot(const QSqlQuery &query);

        void         switchDialogFinishedSlot(int result);
        void           hostDialogFinishedSlot(int result);
        void  bannedScannerDialogFinishedSlot(int result);
        void allowedScannerDialogFinishedSlot(int result);

        void on_tableView_doubleClicked(const QModelIndex &index);

        void on_pbApply_clicked();
        void on_pbLoad_clicked();

        void on_pbCreate_clicked();
        void on_pbRemove_clicked();
    };
}
