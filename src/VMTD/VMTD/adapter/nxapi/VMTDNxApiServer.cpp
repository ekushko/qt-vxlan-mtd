#include "VMTDNxApiServer.h"
#include "VMTDNxApiServerForm.h"

#include "../../VMTDVarname.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDNxApiServer::VMTDNxApiServer(QObject *parent,
                                     VMTDSettings *settings,
                                     VMTDDeviceManager *manager)
        : QObject(parent)
        , m_settings(settings)
        , m_manager(manager)
    {
        m_netManager = new QNetworkAccessManager(this);
    }

    VMTDNxApiServer::~VMTDNxApiServer()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QWidget *VMTDNxApiServer::form() const
    {
        return m_form;
    }
    void VMTDNxApiServer::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiServerForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    bool VMTDNxApiServer::isListening() const
    {
        return m_isListening;
    }

    const QVector<VMTDNxApiAdapter *> &VMTDNxApiServer::adapters() const
    {
        return m_adapters;
    }

    void VMTDNxApiServer::startListenSlot()
    {
        if (m_isListening)
        {
            m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Server already listen!");
            return;
        }

        for (const auto &url : m_manager->switchUrls())
        {
            auto adapter = new VMTDNxApiAdapter(this, m_settings, m_netManager, url);
            m_adapters.append(adapter);

            emit adapterCreatedSignal(adapter);
        }

        m_isListening = true;

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Start listening...");
    }
    void VMTDNxApiServer::stopListenSlot()
    {
        if (!m_isListening)
        {
            m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Server is not listening!");
            return;
        }

        for (auto adapter : m_adapters)
            emit adapterRemovedSignal(adapter);

        qDeleteAll(m_adapters);
        m_adapters.clear();

        m_isListening = false;

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Listening finished!");
    }
    void VMTDNxApiServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }
}
