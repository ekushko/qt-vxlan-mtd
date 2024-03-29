#include "VMTDHostClient.h"
#include "VMTDHostClientForm.h"

#include "../../VMTDVarname.h"

namespace VMTDLib
{
    VMTDHostClient::VMTDHostClient(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_socket = new QWebSocket(m_settings->debugName(), QWebSocketProtocol::VersionLatest, this);
        connect(m_socket, &QWebSocket::textMessageReceived,
                this, &VMTDHostClient::textMessageReceivedSlot);
        connect(m_socket, &QWebSocket::connected, this, &VMTDHostClient::connectedSlot);
        connect(m_socket, &QWebSocket::disconnected, this, &VMTDHostClient::disconnectedSlot);
        connect(m_socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>
                (&QWebSocket::error), this, &VMTDHostClient::errorSlot);
        connect(m_socket, &QWebSocket::stateChanged, this, &VMTDHostClient::stateChangedSlot);

        m_reconnectTimer.setParent(this);
        connect(&m_reconnectTimer, &QTimer::timeout, this, &VMTDHostClient::reconnectTimerTickSlot);

        if (m_settings->shouldReconnect())
            m_reconnectTimer.start(m_settings->reconnectInterval());
    }

    VMTDHostClient::~VMTDHostClient()
    {
        if (m_form != nullptr)
            m_form->deleteLater();

        disconnectSocketSlot();

        m_socket->deleteLater();
    }

    QWidget *VMTDHostClient::form() const
    {
        return m_form;
    }
    void VMTDHostClient::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDHostClientForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDHostClient::settings() const
    {
        return m_settings;
    }

    QWebSocket *VMTDHostClient::socket() const
    {
        return m_socket;
    }

    QString VMTDHostClient::stateString() const
    {
        QString state;

        if (m_socket->state() != QAbstractSocket::ConnectedState)
            state = "Connected";
        else
            state = "Disconnected";

        return state;
    }

    void VMTDHostClient::connectSocketSlot()
    {
        if (m_socket->state() != QAbstractSocket::UnconnectedState)
            return;

        QUrl url;
        url.setHost(m_settings->serverIp());
        url.setPort(m_settings->serverPort());
        url.setScheme("ws");

        m_socket->open(url);

        emit socketConnectedSignal(m_socket);
    }
    void VMTDHostClient::disconnectSocketSlot()
    {
        if (m_socket->state() == QAbstractSocket::UnconnectedState)
            return;

        emit socketDisconnectedSignal(m_socket);

        m_socket->close();
    }
    void VMTDHostClient::reconnectSocketSlot()
    {
        disconnectedSlot();
        connectSocketSlot();
    }

    void VMTDHostClient::sendDataSlot(const QString &ip, int port, const QString &data)
    {
        if (QHostAddress(m_socket->peerAddress().toIPv4Address()).toString() == ip
            && m_socket->peerPort() == port)
        {
            return;
        }

        if (m_socket->state() != QAbstractSocket::ConnectedState)
            return;

        m_socket->sendTextMessage(data);

        auto debugString = QString("Sended to {%1:%2}: %3")
                           .arg(ip).arg(port)
                           .arg(data);

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);
    }


    void VMTDHostClient::textMessageReceivedSlot(const QString &data)
    {
        const auto ip = QHostAddress(m_socket->peerAddress().toIPv4Address()).toString();
        const auto port = m_socket->peerPort();

        const auto debugString = QString("Received from {%1:%2}: %3\n")
                                 .arg(ip).arg(port)
                                 .arg(data);

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);

        emit receiveMessageSignal(ip, port, data);
    }

    void VMTDHostClient::connectedSlot()
    {
        const auto debugString = QString("Socket connected!");

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);
    }
    void VMTDHostClient::disconnectedSlot()
    {
        const auto debugString = QString("Socket disconnected!");

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);
    }

    void VMTDHostClient::errorSlot(QAbstractSocket::SocketError error)
    {
        const auto errorString = QString("Error {%1:%2}: %3")
                                 .arg(m_settings->serverIp())
                                 .arg(m_settings->serverPort())
                                 .arg(QVariant::fromValue(error).toString());

        emit showDebugSignal(m_socket, QTime::currentTime(), errorString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + errorString);
    }

    void VMTDHostClient::stateChangedSlot(QAbstractSocket::SocketState state)
    {
        const auto debugString = QString("{%1:%2}: ")
                                 .arg(m_settings->serverIp())
                                 .arg(m_settings->serverPort())
                                 + QVariant::fromValue(state).toString();

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);
    }

    void VMTDHostClient::reconnectTimerTickSlot()
    {
        if (!m_settings->shouldReconnect())
            return;

        if (m_socket->state() != QAbstractSocket::ConnectedState
            && m_socket->state() != QAbstractSocket::ConnectingState
            && m_socket->state() != QAbstractSocket::HostLookupState)
        {
            const auto debugString = QString("Socket is not connected. Trying to reconnect");

            connectSocketSlot();

            emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
            m_settings->debugOut(VN_S(VMTDHostClient) + " | " + debugString);
        }
    }
}
