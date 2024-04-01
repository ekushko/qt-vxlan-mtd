#include "VMTDClient.h"
#include "VMTDClientForm.h"

#include "../VMTDVarname.h"

namespace VMTDLib
{
    VMTDClient::VMTDClient(QObject *parent,
                           VMTDSettings *settings,
                           VMTDHostClient *hostClient)
        : QObject(parent)
        , m_settings(settings)
        , m_hostClient(hostClient)
    {
        socketConnectedSlot(m_hostClient->socket());
    }

    VMTDClient::~VMTDClient()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QWidget *VMTDClient::form() const
    {
        return m_form;
    }
    void VMTDClient::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDClientForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    QWebSocket *VMTDClient::socket() const
    {
        return m_socket;
    }

    VMTDHostProtocolHandler *VMTDClient::handler() const
    {
        return m_hostHandler;
    }

    void VMTDClient::socketConnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDClient)));
            return;
        }

        const auto ip = QHostAddress(socket->peerAddress().toIPv4Address()).toString();
        const auto port = socket->peerPort();

        if (m_hostHandler != nullptr && m_hostHandler->hostIp() == ip)
        {
            m_settings->debugOut(QString("%1 | Host handler already created!")
                                 .arg(VN_S(VMTDClient)));
            return;
        }

        m_socket = socket;

        m_hostHandler = new VMTDHostProtocolHandler(this,
                                                    m_settings,
                                                    VMTDProtocolHandler::EnSide::CLIENT,
                                                    ip, port);

        connect(m_hostHandler, &VMTDHostProtocolHandler::sendMessageSignal,
                m_hostClient, &VMTDHostClient::sendDataSlot);
        connect(m_hostClient, &VMTDHostClient::receiveMessageSignal,
                m_hostHandler, &VMTDHostProtocolHandler::receiveMessageSlot);
        connect(m_hostHandler, &VMTDHostProtocolHandler::handleMethodSignal,
                this, &VMTDClient::handleMethodSignal,
                Qt::DirectConnection);

        emit handlerCreatedSignal(m_hostHandler);

        m_settings->debugOut(QString("%1 | Host handler created!")
                             .arg(VN_S(VMTDClient)));
    }
    void VMTDClient::socketDisconnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDClient)));
            return;
        }

        if (m_socket != socket)
        {
            m_settings->debugOut(QString("%1 | Host handler not created!")
                                 .arg(VN_S(VMTDClient)));
            return;
        }

        m_socket = nullptr;

        delete m_hostHandler;

        m_settings->debugOut(QString("%1 | Host handler removed!")
                             .arg(VN_S(VMTDClient)));
    }
}
