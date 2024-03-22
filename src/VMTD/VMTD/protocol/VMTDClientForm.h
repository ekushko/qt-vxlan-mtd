#pragma once

#include "VMTDClient.h"

#include "host/VMTDHostProtocolHandlerForm.h"

namespace Ui
{
    class VMTDClientForm;
}

namespace VMTDLib
{
    class VMTDClientForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDClientForm(QWidget *parent, VMTDClient *client);
        ~VMTDClientForm();

    private:

        Ui::VMTDClientForm *ui;

        VMTDClient *m_client;

        VMTDHostProtocolHandlerForm *m_hostForm = nullptr;

    private slots:

        void handlerCreatedSlot(VMTDProtocolHandler *handler);
        void handlerRemovedSlot(VMTDProtocolHandler *handler);
    };
}
