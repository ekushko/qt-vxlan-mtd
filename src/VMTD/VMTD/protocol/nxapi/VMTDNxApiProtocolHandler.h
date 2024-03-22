#pragma once

#include "../VMTDProtocolHandler.h"
#include "../../VMTDAlias.h"

#include "../../adapter/nxapi/VMTDNxApiAdapter.h"

namespace VMTDLib
{
    class VMTDNxApiProtocolHandlerForm;

    class VMTDNxApiProtocolHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDNxApiProtocolHandler(QObject *parent,
                                 VMTDSettings *settings,
                                 VMTDNxApiAdapter *adapter);
        ~VMTDNxApiProtocolHandler();

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString url() const;

        int queueLength() const override;

    signals:

        void updateUrlOnlineSignal(const QString &url, bool isOnline);

        void executeCommandListSignal(const CommandList &commands);

        void checkConnectionSignal();

    public slots:

        void appendCommandListSlot(const CommandList &commands);

        void commandListExecutedSlot(bool isOnline, bool hasError);

        void clearQueueSlot() override;

    private:

        QPointer<VMTDNxApiProtocolHandlerForm> m_form;

        VMTDNxApiAdapter *m_adapter;

        QQueue<QStringList> m_queue;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}
