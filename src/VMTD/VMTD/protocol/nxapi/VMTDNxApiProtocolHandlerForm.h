#pragma once

#include "VMTDNxApiProtocolHandler.h"
#include "../VMTDProtocolHandlerForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDNxApiProtocolHandlerForm;
}

namespace VMTDLib
{
    class VMTDNxApiProtocolHandlerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiProtocolHandlerForm(QWidget *parent, VMTDNxApiProtocolHandler *handler);
        ~VMTDNxApiProtocolHandlerForm();

    signals:

        void appendCommandListSignal(const CommandList &commands);

    private:

        Ui::VMTDNxApiProtocolHandlerForm *ui;

        VMTDNxApiProtocolHandler *m_handler;

        VMTDProtocolHandlerForm *m_handlerForm;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void on_pbSend_clicked();
    };
}
