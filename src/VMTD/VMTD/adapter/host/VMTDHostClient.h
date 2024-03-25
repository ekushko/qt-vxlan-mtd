#pragma once

#include "../../VMTDSettings.h"

#include <QWebSocket>
#include <QPointer>
#include <QTimer>

namespace VMTDLib
{
    class VMTDHostClientForm;

    class VMTDHostClient : public QObject
    {
        Q_OBJECT

    public:

        VMTDHostClient(QObject *parent, VMTDSettings *settings);
        ~VMTDHostClient();

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        VMTDSettings *settings() const;

        QWebSocket *socket() const;

        QString stateString() const;

    signals:

        void receiveMessageSignal(QWebSocket *socket, const QString &data);

        void showDebugSignal(QWebSocket *socket, const QTime &time, const QString &text);

        void socketConnectedSignal(QWebSocket *socket);
        void socketDisconnectedSignal(QWebSocket *socket);

    public slots:

        void    connectSocketSlot();
        void disconnectSocketSlot();
        void  reconnectSocketSlot();

        void sendDataSlot(QWebSocket *socket, const QString &data);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDHostClientForm> m_form;

        QWebSocket *m_socket;

        QTimer m_reconnectTimer;

    private slots:

        void textMessageReceivedSlot(const QString &data);

        void    connectedSlot();
        void disconnectedSlot();

        void        errorSlot(QAbstractSocket::SocketError error);
        void stateChangedSlot(QAbstractSocket::SocketState stateString);

        void reconnectTimerTickSlot();
    };
}
