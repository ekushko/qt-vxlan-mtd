#pragma once

#include "VMTDHostProtocolHandler.h"
#include "../VMTDProtocolHandlerForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDHostProtocolHandlerForm;
}

namespace VMTDLib
{
    class VMTDHostProtocolHandlerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDHostProtocolHandlerForm(QWidget *parent, VMTDHostProtocolHandler *handler);
        ~VMTDHostProtocolHandlerForm();

    signals:

        void appendRequestListSignal(const RequestList &requests);

    private:

        Ui::VMTDHostProtocolHandlerForm *ui;

        VMTDHostProtocolHandler *m_handler;

        VMTDProtocolHandlerForm *m_handlerForm;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void on_pbSend_clicked();
    };
}
