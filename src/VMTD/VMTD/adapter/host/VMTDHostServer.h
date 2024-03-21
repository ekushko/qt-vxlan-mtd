#pragma once

#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>

#include "../../VMTDSettings.h"

namespace VMTDLib
{
    class VMTDHostServerForm;

    class VMTDHostServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDHostServer(QObject *parent, VMTDSettings *settings);
        ~VMTDHostServer();

        VMTDSettings *settings() const;

        QWebSocketServer *wsServer() const;

        QList<QWebSocket *> WsClientSockets;

        QString stateString() const;

    signals:

        void clientConnectedSignal(QWebSocket *socket);
        void clientDisconnectedSignal(QWebSocket *socket);

        void receiveMessageSignal(QWebSocket *socket, const QString &data);

        void showDebugSignal(QWebSocket *socket, const QTime &time, const QString &text);

    public slots:

        void showFormSlot();

        void   startListenSlot();
        void    stopListenSlot();
        void restartListenSlot();

        void sendMessageSlot(QWebSocket *socket, const QString &data);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDHostServerForm> m_form;

        QWebSocketServer *m_wsServer;

    private slots:

        void newConnectionSlot();

        void textMessageReceivedSlot(const QString &data);

        void errorSlot(QAbstractSocket::SocketError error);

        void    connectedSlot();
        void disconnectedSlot();
    };
}
