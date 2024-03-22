#pragma once

#include "../adapter/host/VMTDHostClient.h"

#include "host/VMTDHostProtocolHandler.h"

namespace VMTDLib
{
    class VMTDClientForm;

    class VMTDClient : public QObject
    {
        Q_OBJECT

    public:

        VMTDClient(QObject *parent,
                   VMTDSettings *settings,
                   VMTDHostClient *hostClient);
        ~VMTDClient();

        void showForm();

    signals:

        void handleMethodSignal(const QString &method, const QJsonObject &params, bool &result);

        void handlerCreatedSignal(VMTDProtocolHandler *handler);
        void handlerRemovedSignal(VMTDProtocolHandler *handler);

    private:

        VMTDSettings *m_settings;

        VMTDHostClient *m_hostClient;
        QWebSocket *m_socket;
        VMTDHostProtocolHandler *m_hostHandler;

    private slots:

        void socketConnectedSlot(QWebSocket *socket);
        void socketDisconnectedSlot(QWebSocket *socket);
    };
}
