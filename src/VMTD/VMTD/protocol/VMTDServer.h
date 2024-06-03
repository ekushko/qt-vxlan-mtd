#pragma once

#include "../adapter/nxapi/VMTDNxApiServer.h"
#include "../adapter/host/VMTDHostServer.h"

#include "nxapi/VMTDNxApiProtocolHandler.h"
#include "host/VMTDHostProtocolHandler.h"

namespace VMTDLib
{
    class VMTDServerForm;

    class VMTDServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDServer(QObject *parent,
                   VMTDSettings *settings,
                   VMTDNxApiServer *nxApiServer,
                   VMTDHostServer *hostServer);
        ~VMTDServer();

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        // ДАННЫЕ

        const QMap<QString, VMTDNxApiProtocolHandler *> &nxApiHandlers() const;
        const QMap<QString, VMTDHostProtocolHandler *> &hostHandlers() const;

    signals:

        void updateUrlOnlineSignal(const QString &url, bool isOnline);
        void updateIpOnlineSignal(const QString &ip, bool isOnline);

        void handlerCreatedSignal(VMTDProtocolHandler *handler);
        void handlerRemovedSignal(VMTDProtocolHandler *handler);

        void handleResultSignal(const QJsonValue &result);

    public slots:

        void appendRequestListSlot(const QString &ip, const RequestList &requests);
        void appendCommandListSlot(const QString &url, const CommandList &commands);

    private:

        QPointer<VMTDServerForm> m_form;

        VMTDSettings *m_settings;

        VMTDNxApiServer *m_nxApiServer;
        QMap<QString, VMTDNxApiProtocolHandler *> m_nxApiHandlers;

        VMTDHostServer *m_hostServer;
        QMap<QString, VMTDHostProtocolHandler *> m_hostHandlers;

        QTimer m_checkConnectionTimer;

    private slots:

        void adapterCreatedSlot(VMTDNxApiAdapter *adapter);
        void adapterRemovedSlot(VMTDNxApiAdapter *adapter);

        void clientConnectedSlot(QWebSocket *socket);
        void clientDisconnectedSlot(QWebSocket *socket);

        void checkConnectionTimerTickSlot();
    };
}
