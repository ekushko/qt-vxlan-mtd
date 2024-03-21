#pragma once

#include "VMTDNxApiServer.h"
#include "VMTDNxApiAdapterForm.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDNxApiServerForm;
}

namespace VMTDLib
{
    class VMTDNxApiServerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiServerForm(QWidget *parent, VMTDNxApiServer *server);
        ~VMTDNxApiServerForm();

    private:

        Ui::VMTDNxApiServerForm *ui;

        VMTDNxApiServer *m_server;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

        QMap<VMTDNxApiAdapter *, VMTDNxApiAdapterForm *> m_adapterForms;

    private slots:

        void uiTimerTickSlot();

        void adapterCreatedSlot(VMTDNxApiAdapter *adapter);
        void adapterRemovedSlot(VMTDNxApiAdapter *adapter);

        void on_pbShowDetailedState_clicked();
    };
}
