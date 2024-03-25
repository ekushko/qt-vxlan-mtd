#include "VMTDServer.h"
#include "VMTDServerForm.h"

#include "../VMTDVarname.h"

namespace VMTDLib
{
    VMTDServer::VMTDServer(QObject *parent,
                           VMTDSettings *settings,
                           VMTDNxApiServer *nxApiServer,
                           VMTDHostServer *hostServer)
        : QObject(parent)
        , m_settings(settings)
        , m_nxApiServer(nxApiServer)
        , m_hostServer(hostServer)
    {
        connect(m_nxApiServer, &VMTDNxApiServer::adapterCreatedSignal,
                this, &VMTDServer::adapterCreatedSlot);
        connect(m_nxApiServer, &VMTDNxApiServer::adapterRemovedSignal,
                this, &VMTDServer::adapterRemovedSlot);

        connect(m_hostServer, &VMTDHostServer::clientConnectedSignal,
                this, &VMTDServer::clientConnectedSlot);
        connect(m_hostServer, &VMTDHostServer::clientDisconnectedSignal,
                this, &VMTDServer::clientDisconnectedSlot);

        m_checkConnectionTimer.setParent(this);
        m_checkConnectionTimer.setInterval(m_settings->checkConnectionInterval());
        connect(&m_checkConnectionTimer, &QTimer::timeout,
                this, &VMTDServer::checkConnectionTimerTickSlot);

        if (m_settings->shouldCheckConnection())
            m_checkConnectionTimer.start();
    }

    VMTDServer::~VMTDServer()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QWidget *VMTDServer::form() const
    {
        return m_form;
    }
    void VMTDServer::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDServerForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    const QMap<QString, VMTDNxApiProtocolHandler *> &VMTDServer::nxApiHandlers() const
    {
        return m_nxApiHandlers;
    }

    const QMap<QString, VMTDHostProtocolHandler *> &VMTDServer::hostHandlers() const
    {
        return m_hostHandlers;
    }

    void VMTDServer::appendRequestListSlot(const QString &ip, const RequestList &requests)
    {
        if (m_hostHandlers.contains(ip))
        {
            m_hostHandlers.value(ip)->appendRequestListSlot(requests);
        }
        else
        {
            m_settings->debugOut(QString("%1 | Host %2 handler not found!")
                                 .arg(VN_S(VMTDServer))
                                 .arg(ip));
        }
    }
    void VMTDServer::appendCommandListSlot(const QString &url, const CommandList &commands)
    {
        if (m_nxApiHandlers.contains(url))
        {
            m_nxApiHandlers.value(url)->appendCommandListSlot(commands);
        }
        else
        {
            m_settings->debugOut(QString("%1 | NX-API %2 handler not found!")
                                 .arg(VN_S(VMTDServer))
                                 .arg(url));
        }
    }

    void VMTDServer::adapterCreatedSlot(VMTDNxApiAdapter *adapter)
    {
        if (adapter == nullptr)
        {
            m_settings->debugOut(QString("%1 | NX-API Adapter is NULL!")
                                 .arg(VN_S(VMTDServer)));
            return;
        }

        const auto url = adapter->url().toString(QUrl::RemoveUserInfo);

        if (m_nxApiHandlers.contains(url))
        {
            m_settings->debugOut(QString("%1 | NX-API handler for %2 already created!")
                                 .arg(VN_S(VMTDServer))
                                 .arg(url));
            return;
        }

        auto handler = new VMTDNxApiProtocolHandler(this, m_settings, adapter);
        m_nxApiHandlers[url] = handler;

        connect(handler, &VMTDNxApiProtocolHandler::executeCommandListSignal,
                adapter, &VMTDNxApiAdapter::executeCommandListSlot);
        connect(adapter, &VMTDNxApiAdapter::commandListExecutedSignal,
                handler, &VMTDNxApiProtocolHandler::commandListExecutedSlot);
        connect(handler, &VMTDNxApiProtocolHandler::updateUrlOnlineSignal,
                this, &VMTDServer::updateUrlOnlineSignal);

        emit handlerCreatedSignal(handler);

        m_settings->debugOut(QString("%1 | NX-API handler for %2 created!")
                             .arg(VN_S(VMTDServer))
                             .arg(url));
    }
    void VMTDServer::adapterRemovedSlot(VMTDNxApiAdapter *adapter)
    {
        if (adapter == nullptr)
        {
            m_settings->debugOut(QString("%1 | NX-API Adapter is NULL!")
                                 .arg(VN_S(VMTDServer)));
            return;
        }

        const auto url = adapter->url().toString(QUrl::RemoveUserInfo);

        if (!m_nxApiHandlers.contains(url))
        {
            m_settings->debugOut(QString("%1 | NX-API handler for %2 not created!")
                                 .arg(VN_S(VMTDServer))
                                 .arg(url));
            return;
        }

        auto handler = m_nxApiHandlers[url];

        m_nxApiHandlers.remove(url);
        emit handlerRemovedSignal(handler);
        delete handler;

        m_settings->debugOut(QString("%1 | NX-API handler for %2 removed!")
                             .arg(VN_S(VMTDServer))
                             .arg(url));
    }

    void VMTDServer::clientConnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        const auto ip = QHostAddress(socket->peerAddress().toIPv4Address()).toString();

        if (m_hostHandlers.contains(ip))
        {
            m_settings->debugOut(QString("%1 | Host handler for %2 already created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(ip));
            return;
        }

        auto handler = new VMTDHostProtocolHandler(this, m_settings,
                                                   VMTDProtocolHandler::EnSide::SERVER,
                                                   socket);
        m_hostHandlers[ip] = handler;

        connect(handler, &VMTDHostProtocolHandler::sendMessageSignal,
                m_hostServer, &VMTDHostServer::sendMessageSlot);
        connect(m_hostServer, &VMTDHostServer::receiveMessageSignal,
                handler, &VMTDHostProtocolHandler::receiveMessageSlot);
        connect(handler, &VMTDHostProtocolHandler::updateIpOnlineSignal,
                this, &VMTDServer::updateIpOnlineSignal);

        emit handlerCreatedSignal(handler);

        m_settings->debugOut(QString("%1 | Host handler for %2 created!")
                             .arg(VN_S(VMTDProtocol))
                             .arg(ip));
    }
    void VMTDServer::clientDisconnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        const auto ip = QHostAddress(socket->peerAddress().toIPv4Address()).toString();

        if (!m_hostHandlers.contains(ip))
        {
            m_settings->debugOut(QString("%1 | Host handler for %2 not created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(ip));
            return;
        }

        auto handler = m_hostHandlers[ip];

        m_hostHandlers.remove(ip);
        emit handlerRemovedSignal(handler);
        delete handler;

        m_settings->debugOut(QString("%1 | Host handler for %2 removed!")
                             .arg(VN_S(VMTDServer))
                             .arg(ip));
    }

    void VMTDServer::checkConnectionTimerTickSlot()
    {
        for (auto handler : m_nxApiHandlers.values())
            handler->checkConnection();

        for (auto handler : m_hostHandlers.values())
            handler->checkConnection();
    }
}
