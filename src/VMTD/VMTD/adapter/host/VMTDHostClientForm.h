#pragma once

#include "VMTDHostClient.h"
#include "VMTDHostAdapterForm.h"

namespace Ui
{
    class VMTDHostClientForm;
}

namespace VMTDLib
{
    class VMTDHostClientForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDHostClientForm(QWidget *parent, VMTDHostClient *client);
        ~VMTDHostClientForm();

    private:

        void initializeView();
        void     updateView();

        Ui::VMTDHostClientForm *ui;

        VMTDHostClient *m_client;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

        VMTDHostAdapterForm *m_adapterForm;

    private slots:

        void uiTimerTickSlot();

        void on_pbShowDetailedState_clicked();
    };
}
