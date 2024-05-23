#include "VMTDController.h"

#include "VMTDVarname.h"

#include <QCoreApplication>

namespace VMTDLib
{
    VMTDController::VMTDController(QObject *parent, const QString &systemName)
        : QThread(parent)
    {
        qRegisterMetaType<RequestList>(VN(RequestList));
        qRegisterMetaType<CommandList>(VN(CommandList));

        connect(this, &VMTDController::started, this, &VMTDController::startedSlot);
        connect(this, &VMTDController::finished, this, &VMTDController::finishedSlot);

        m_settings = new VMTDSettings(this, systemName);
        connect(m_settings, &VMTDSettings::restartSignal, this, &VMTDController::restartSlot);

        m_manager = new VMTDDeviceManager(this, m_settings);
        connect(m_manager, &VMTDDeviceManager::restartSignal, this, &VMTDController::restartSlot);

        m_engine = new VMTDEngine(this, m_settings, m_manager);
    }

    VMTDController::~VMTDController()
    {
        if (isRunning())
            stopController();

        delete m_engine;

        delete m_manager;

        delete m_settings;
    }

    VMTDSettings *VMTDController::settings() const
    {
        return m_settings;
    }

    VMTDDeviceManager *VMTDController::manager() const
    {
        return m_manager;
    }

    VMTDNxApiServer *VMTDController::nxApiServer() const
    {
        return m_nxApiServer;
    }

    VMTDHostServer *VMTDController::hostServer() const
    {
        return m_hostServer;
    }

    VMTDHostClient *VMTDController::hostClient() const
    {
        return m_hostClient;
    }

    VMTDServer *VMTDController::server() const
    {
        return m_server;
    }

    VMTDClient *VMTDController::client() const
    {
        return m_client;
    }

    VMTDEngine *VMTDController::engine() const
    {
        return m_engine;
    }

    VMTDConfigurator *VMTDController::configurator() const
    {
        return m_configurator;
    }

    void VMTDController::startController()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Starting...");

        start();
    }
    void VMTDController::stopController()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Stopping...");

        quit();
        wait();
    }

    void VMTDController::run()
    {
        m_nodeType = m_settings->nodeType();

        m_hostClient = new VMTDHostClient(nullptr, m_settings);
        connect(this, &VMTDController::finished, m_hostClient, &VMTDHostClient::deleteLater);
        m_hostClient->connectSocketSlot();

        m_client = new VMTDClient(nullptr, m_settings, m_hostClient);
        connect(this, &VMTDController::finished, m_hostClient, &VMTDHostClient::deleteLater);

        m_configurator = new VMTDConfigurator(nullptr, m_settings);
        connect(this, &VMTDController::finished, m_configurator, &VMTDConfigurator::deleteLater);
        connect(m_client, &VMTDClient::handleMethodSignal,
                m_configurator, &VMTDConfigurator::handleMethodSlot,
                Qt::DirectConnection);

        if (m_nodeType == VMTDNodeType::SERVER)
        {
            m_nxApiServer = new VMTDNxApiServer(nullptr, m_settings, m_manager);
            connect(this, &VMTDController::finished, m_nxApiServer, &VMTDNxApiServer::deleteLater);
            m_nxApiServer->startListenSlot();

            m_hostServer = new VMTDHostServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_hostServer, &VMTDHostServer::deleteLater);
            m_hostServer->startListenSlot();

            m_server = new VMTDServer(nullptr, m_settings, m_nxApiServer, m_hostServer);
            connect(this, &VMTDController::finished, m_hostServer, &VMTDServer::deleteLater);
            connect(m_server, &VMTDServer::updateUrlOnlineSignal,
                    m_manager, &VMTDDeviceManager::updateUrlOnlineSlot);
            connect(m_server, &VMTDServer::updateIpOnlineSignal,
                    m_manager, &VMTDDeviceManager::updateIpOnlineSlot);
            connect(m_engine, &VMTDEngine::appendCommandListSignal,
                    m_server, &VMTDServer::appendCommandListSlot);
            connect(m_engine, &VMTDEngine::appendRequestListSignal,
                    m_server, &VMTDServer::appendRequestListSlot);

            m_engine->startEngine();
        }

        exec();

        if (m_nodeType == VMTDNodeType::SERVER)
        {
            m_engine->stopEngine();

            m_hostServer->stopListenSlot();

            m_nxApiServer->stopListenSlot();
        }

        m_hostClient->disconnectSocketSlot();
    }

    void VMTDController::restartSlot()
    {
        const auto shouldBeStarted = isRunning();

        if (shouldBeStarted)
            stopController();

        emit restartedSignal();

        if (shouldBeStarted)
            startController();
    }

    void VMTDController::startedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Started");
    }
    void VMTDController::finishedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Stopped");
    }
}
