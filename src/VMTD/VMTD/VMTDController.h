#pragma once

#include "VMTDSettings.h"

#include "device/VMTDDeviceManager.h"

#include "adapter/nxapi/VMTDNxApiServer.h"
#include "adapter/host/VMTDHostServer.h"
#include "adapter/host/VMTDHostClient.h"

#include "protocol/VMTDServer.h"
#include "protocol/VMTDClient.h"

#include "engine/VMTDEngine.h"
#include "engine/VMTDConfigurator.h"
#include "engine/VMTDWatchdog.h"

#include <QThread>

namespace VMTDLib
{
    class VMTDController : public QThread
    {
        Q_OBJECT

    public:

        VMTDController(QObject *parent, const QString &systemName);
        ~VMTDController();

        VMTDSettings *settings() const;

        VMTDDeviceManager *manager() const;

        VMTDNxApiServer *nxApiServer() const;

        VMTDHostServer *hostServer() const;

        VMTDHostClient *hostClient() const;

        VMTDServer *server() const;

        VMTDClient *client() const;

        VMTDEngine *engine() const;

        VMTDConfigurator *configurator() const;

        VMTDWatchdog *watchdog() const;

    signals:

        void restartedSignal();

    public slots:

        void startController();
        void  stopController();

    protected:

        void run() override;

    private:

        VMTDNodeType m_nodeType;

        VMTDSettings *m_settings;

        VMTDNxApiServer *m_nxApiServer;

        VMTDHostServer *m_hostServer;

        VMTDHostClient *m_hostClient;

        VMTDServer *m_server;

        VMTDClient *m_client;

        VMTDDeviceManager *m_manager;

        VMTDEngine *m_engine;

        VMTDConfigurator *m_configurator;

        VMTDWatchdog *m_watchdog;

    private slots:

        void restartSlot();

        void  startedSlot();
        void finishedSlot();
    };
}
