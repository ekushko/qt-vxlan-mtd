#pragma once

#include "VMTDServer.h"

#include "nxapi/VMTDNxApiProtocolHandlerForm.h"
#include "host/VMTDHostProtocolHandlerForm.h"

namespace Ui
{
    class VMTDServerForm;
}

namespace VMTDLib
{
    class VMTDServerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDServerForm(QWidget *parent, VMTDServer *server);
        ~VMTDServerForm();

    private:

        Ui::VMTDServerForm *ui;

        VMTDServer *m_server;

        QMap<VMTDProtocolHandler *, VMTDNxApiProtocolHandlerForm *> m_nxApiForms;
        QMap<VMTDProtocolHandler *, VMTDHostProtocolHandlerForm *>   m_hostForms;

    private slots:

        void handlerCreatedSlot(VMTDProtocolHandler *handler);
        void handlerRemovedSlot(VMTDProtocolHandler *handler);
    };
}
