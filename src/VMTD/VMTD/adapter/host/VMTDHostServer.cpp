#include "VMTDHostServer.h"
#include "VMTDHostServerForm.h"

#include "../../VMTDVarname.h"

namespace VMTDLib
{
    VMTDHostServer::VMTDHostServer(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_wsServer = new QWebSocketServer("VMTDHostServer", QWebSocketServer::NonSecureMode, this);
        connect(m_wsServer, &QWebSocketServer::newConnection, this, &VMTDHostServer::newConnectionSlot);
    }

    VMTDHostServer::~VMTDHostServer()
    {
        if (m_form != nullptr)
            m_form->deleteLater();

        if (m_wsServer->isListening())
            stopListenSlot();

        delete m_wsServer;
    }

    VMTDSettings *VMTDHostServer::settings() const
    {
        return m_settings;
    }

    QWebSocketServer *VMTDHostServer::wsServer() const
    {
        return m_wsServer;
    }

    QString VMTDHostServer::stateString() const
    {
        QString state;

        if (m_wsServer->isListening())
            state = QString("Opened. Clients: %1").arg(WsClientSockets.count());
        else
            state = "Closed";

        return state;
    }

    void VMTDHostServer::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDHostServerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDHostServer::startListenSlot()
    {
        QString debugString;

        if (m_wsServer->isListening())
        {
            debugString = "Server already started";
            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
            m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

            return;
        }

        const auto result = m_wsServer->listen(QHostAddress::Any, m_settings->serverPort());

        if (result)
        {
            debugString = "Server started. Listen port: "
                          + QString::number(m_settings->localPort());
        }
        else
        {
            debugString = "Some problem when starting server. See errors.";
        }

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);
    }
    void VMTDHostServer::stopListenSlot()
    {
        QString debugString;

        if (m_wsServer->isListening())
        {
            m_wsServer->close();

            for (auto socket : qAsConst(WsClientSockets))
                socket->close();

            WsClientSockets.clear();

            debugString = "Server stopped";
        }
        else
        {
            debugString = "Server already stopped";
        }

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);
    }
    void VMTDHostServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }

    void VMTDHostServer::sendMessageSlot(QWebSocket *socket, const QString &data)
    {
        if (socket == nullptr)
            return;

        if (socket->state() != QAbstractSocket::ConnectedState)
            return;

        socket->sendTextMessage(data);

        auto debugString = QString("Sended to {%1:%2}: %3\n")
                           .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                           .arg(socket->peerPort())
                           .arg(data);

        emit showDebugSignal(socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);
    }

    void VMTDHostServer::newConnectionSlot()
    {
        QString debugString;

        QWebSocket *socket = m_wsServer->nextPendingConnection();

        if (socket == nullptr)
        {
            debugString = "Incorrect client socket in {newConnection} slot-function";
            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
            m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

            return;
        }

        WsClientSockets.append(socket);

        connect(socket, &QWebSocket::textMessageReceived,
                this, &VMTDHostServer::textMessageReceivedSlot);

        connect(socket, &QWebSocket::disconnected, socket, &QWebSocket::deleteLater);
        connect(socket, &QWebSocket::disconnected, this, &VMTDHostServer::disconnectedSlot);
        connect(socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                this, &VMTDHostServer::errorSlot);

        debugString = QString("Handled {newConnection} from: %1:%2")
                      .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                      .arg(socket->peerPort());
        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

        emit clientConnectedSignal(socket);
    }

    void VMTDHostServer::textMessageReceivedSlot(const QString &data)
    {
        auto socket = dynamic_cast<QWebSocket *>(sender());

        const auto debugString = QString("Received from {%1:%2}: %3\n")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort())
                                 .arg(data);

        emit showDebugSignal(socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

        emit receiveMessageSignal(socket, data);
    }

    void VMTDHostServer::errorSlot(QAbstractSocket::SocketError error)
    {
        auto socket = dynamic_cast<QWebSocket *>(sender());

        if (socket == nullptr)
            return;

        const auto errorString = QString("Error {%1:%2}: %3")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort())
                                 .arg(QVariant::fromValue(error).toString());

        emit showDebugSignal(socket, QTime::currentTime(), errorString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + errorString);
    }

    void VMTDHostServer::connectedSlot()
    {
        // do nothing
    }
    void VMTDHostServer::disconnectedSlot()
    {
        QString debugString;

        auto socket = dynamic_cast<QWebSocket *>(sender());

        if (socket == nullptr)
        {
            debugString = "Incorrect client socket in {disconnected} slot-function";

            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
            m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

            return;
        }

        debugString = QString("Handled {disconnected} from: %1:%2")
                      .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                      .arg(socket->peerPort());

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDHostServer) + " | " + debugString);

        emit clientDisconnectedSignal(socket);

        WsClientSockets.removeOne(socket);
    }
}
