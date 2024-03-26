#include "VMTDConfigurator.h"
#include "VMTDConfiguratorForm.h"

#include "../VMTDVarname.h"
#include "../VMTDMethod.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

namespace VMTDLib
{
    VMTDConfigurator::VMTDConfigurator(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }

    VMTDConfigurator::~VMTDConfigurator()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QWidget *VMTDConfigurator::form() const
    {
        return m_form;
    }
    void VMTDConfigurator::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDConfiguratorForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDConfigurator::settings() const
    {
        return m_settings;
    }

    const QString &VMTDConfigurator::netplan1() const
    {
        return m_netplan1;
    }
    const QString &VMTDConfigurator::netplan2() const
    {
        return m_netplan2;
    }
    const QString &VMTDConfigurator::hosts() const
    {
        return m_hosts;
    }

    void VMTDConfigurator::handleMethodSlot(const QString &method,
                                            const QJsonObject &params,
                                            bool &result)
    {
        m_settings->debugOut(QString("%1 | Start handling method: %2")
                             .arg(VN_S(VMTDConfigurator))
                             .arg(method));

        if (method == MTH_SETUP_INTERFACE_1)
        {
            result = handleSetupInterface1(params);
        }
        else if (method == MTH_SETUP_INTERFACE_2)
        {
            result = handleSetupInterface2(params);
        }
        else if (method == MTH_SETUP_HOSTS)
        {
            result = handleSetupHosts(params);
        }
        else if (method == MTH_CLEAR_INTERFACE_1)
        {
            result = handleClearInterface1(params);
        }
        else if (method == MTH_CLEAR_INTERFACE_2)
        {
            result = handleClearInterface2(params);
        }
        else if (method == MTH_CLEAR_HOSTS)
        {
            result = handleClearHosts(params);
        }
        else if (method == MTH_CHECK_CONNECTION)
        {
            result = handleCheckConnection(params);
        }
        else
        {
            result = false;
        }

        m_settings->debugOut(QString("%1 | Method handled %2: %3")
                             .arg(VN_S(VMTDConfigurator))
                             .arg(result ? "successfully" : "with error")
                             .arg(method));
    }

    bool VMTDConfigurator::handleSetupInterface1(const QJsonObject &params)
    {
        m_netplan1 = VLAN_HEADER_TEMPLATE +
                     QString(VLAN_TEMPLATE)
                     .arg(params[PRM_VLAN_ID].toString())
                     .arg(params[PRM_INTERFACE].toString())
                     .arg(params[PRM_IP].toString())
                     .arg(params[PRM_MASK].toInt())
                     .arg(params[PRM_MAC].toString());

        if (params.contains(PRM_ROUTES)
            && params[PRM_ROUTES].isArray())
        {
            m_netplan1 += ROUTE_HEADER_TEMPLATE;

            for (const auto routeVal : params[PRM_ROUTES].toArray())
            {
                const auto &routeObj = routeVal.toObject();

                m_netplan1 += QString(ROUTE_TEMPLATE)
                              .arg(routeObj[PRM_NETWORK].toString())
                              .arg(routeObj[PRM_MASK].toInt())
                              .arg(routeObj[PRM_GATEWAY].toString())
                              .arg(routeObj[PRM_METRIC].toInt());
            }
        }

        return true;
    }
    bool VMTDConfigurator::handleSetupInterface2(const QJsonObject &params)
    {
        m_netplan2 = VLAN_HEADER_TEMPLATE +
                     QString(VLAN_TEMPLATE)
                     .arg(params[PRM_VLAN_ID].toString())
                     .arg(params[PRM_INTERFACE].toString())
                     .arg(params[PRM_IP].toString())
                     .arg(params[PRM_MASK].toInt())
                     .arg(params[PRM_MAC].toString());

        if (params.contains(PRM_ROUTES)
            && params[PRM_ROUTES].isArray())
        {
            m_netplan2 += ROUTE_HEADER_TEMPLATE;

            for (const auto routeVal : params[PRM_ROUTES].toArray())
            {
                const auto &routeObj = routeVal.toObject();

                m_netplan2 += QString(ROUTE_TEMPLATE)
                              .arg(routeObj[PRM_NETWORK].toString())
                              .arg(routeObj[PRM_MASK].toInt())
                              .arg(routeObj[PRM_GATEWAY].toString())
                              .arg(routeObj[PRM_METRIC].toInt());
            }
        }

        return true;
    }
    bool VMTDConfigurator::handleSetupHosts(const QJsonObject &params)
    {
        m_hosts.clear();

        const auto &jsonArr = params[PRM_HOSTS].toArray();

        if (!jsonArr.isEmpty())
        {
            for (auto jsonVal : params[PRM_HOSTS].toArray())
            {
                const auto jsonObj = jsonVal.toObject();

                m_hosts.append(QString("%1 %2\n")
                               .arg(params[PRM_IP].toString())
                               .arg(params[PRM_DOMAIN_NAME].toString()));
            }
        }

        return true;
    }

    bool VMTDConfigurator::handleClearInterface1(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan1.clear();

        return true;
    }
    bool VMTDConfigurator::handleClearInterface2(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan2.clear();

        return true;
    }
    bool VMTDConfigurator::handleClearHosts(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_hosts.clear();

        return true;
    }

    bool VMTDConfigurator::handleApplyNetplan(const QJsonObject &params)
    {
        Q_UNUSED(params)

        // в разработке

        return true;
    }

    bool VMTDConfigurator::handleCheckConnection(const QJsonObject &params)
    {
        Q_UNUSED(params)

        return true;
    }
}
