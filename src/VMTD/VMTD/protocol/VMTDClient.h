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

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        QWebSocket *socket() const;

        VMTDHostProtocolHandler *handler() const;

    signals:

        void handleMethodSignal(const QString &method, const QJsonObject &params, bool &result);

        void handlerCreatedSignal(VMTDProtocolHandler *handler);
        void handlerRemovedSignal(VMTDProtocolHandler *handler);

    private:

        QPointer<VMTDClientForm> m_form;

        VMTDSettings *m_settings;

        VMTDHostClient *m_hostClient;
        QWebSocket *m_socket = nullptr;
        VMTDHostProtocolHandler *m_hostHandler = nullptr;

    private slots:

        void socketConnectedSlot(QWebSocket *socket);
        void socketDisconnectedSlot(QWebSocket *socket);
    };
}
