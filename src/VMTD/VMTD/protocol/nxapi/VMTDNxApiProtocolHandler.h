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
                                 const QString &url);

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

        QString m_url;

        QQueue<QStringList> m_queue;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}
