#include "VMTDParticipant.h"

#include "../VMTDTemplate.h"
#include "../VMTDMethod.h"

namespace VMTDLib
{
    VMTDParticipant::VMTDParticipant(QObject *parent)
        : QObject(parent)
    {
        m_role = EnRole::ENDPOINT;

        m_defaultVlanId = 19;

        m_hostName = "none";
        m_hostIp = "none";
        m_hostInterface = "none";

        m_switchName = "none";
        m_switchUrl = "none";
        m_switchPort = "none";

        m_interface1 = new VMTDInterface();
        m_interface2 = new VMTDInterface();
    }

    VMTDParticipant::~VMTDParticipant()
    {
        delete m_interface1;
        delete m_interface2;
    }

    const QString &VMTDParticipant::enRoleToS(const EnRole &role)
    {
        switch (role)
        {
        // *INDENT-OFF*
        case EnRole::ENDPOINT: RETURN_S("Endpoint");
        case EnRole::GATEWAY : RETURN_S("Gateway");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDParticipant::enRoleToL()
    {
        RETURN_MAP(EnRole, enRoleToS);
    }

    void VMTDParticipant::buildRequests()
    {
        m_requestList.clear();

        // INTERFACE 1
        {
            QJsonObject jsonObj;

            if (m_role == EnRole::ENDPOINT
                || m_role == EnRole::GATEWAY)
            {
                jsonObj[PRM_INTERFACE] = hostInterface();
                jsonObj[PRM_MAC] = m_interface1->mac();
                jsonObj[PRM_IP] = m_interface1->ip();
                jsonObj[PRM_MASK] = m_interface1->mask();
                jsonObj[PRM_VLAN_ID] = m_interface1->vlanId();

                QJsonArray jsonArr;

                for (const auto &route : m_interface1->routes())
                {
                    QJsonObject routeObj;

                    routeObj[PRM_NETWORK] = route.network();
                    routeObj[PRM_MASK] = route.mask();
                    routeObj[PRM_GATEWAY] = route.gateway();
                    routeObj[PRM_METRIC] = route.metric();

                    jsonArr.append(routeObj);
                }

                jsonObj[PRM_ROUTES] = jsonArr;

                m_requestList.append(qMakePair(MTH_SETUP_INTERFACE_1, jsonObj));
            }
            else
            {
                m_requestList.append(qMakePair(MTH_CLEAR_INTERFACE_1, jsonObj));
            }
        }

        // INTERFACE 2
        {
            QJsonObject jsonObj;

            if (m_role == EnRole::GATEWAY)
            {
                jsonObj[PRM_INTERFACE] = hostInterface();
                jsonObj[PRM_MAC] = m_interface2->mac();
                jsonObj[PRM_IP] = m_interface2->ip();
                jsonObj[PRM_MASK] = m_interface2->mask();
                jsonObj[PRM_VLAN_ID] = m_interface2->vlanId();

                QJsonArray jsonArr;

                for (const auto &route : m_interface2->routes())
                {
                    QJsonObject routeObj;

                    routeObj[PRM_NETWORK] = route.network();
                    routeObj[PRM_MASK] = route.mask();
                    routeObj[PRM_GATEWAY] = route.gateway();
                    routeObj[PRM_METRIC] = route.metric();

                    jsonArr.append(routeObj);
                }

                jsonObj[PRM_ROUTES] = jsonArr;

                m_requestList.append(qMakePair(MTH_SETUP_INTERFACE_2, jsonObj));
            }
            else
            {
                m_requestList.append(qMakePair(MTH_CLEAR_INTERFACE_2, jsonObj));
            }
        }


        m_requestList.append(qMakePair(MTH_APPLY_NETPLAN, QJsonObject()));
    }

    void VMTDParticipant::buildCommands()
    {
        m_commandList.clear();

        auto vlans = QString::number(m_defaultVlanId);

        vlans += QString(", %1").arg(m_interface1->vlanId());

        if (m_role == EnRole::GATEWAY)
            vlans += QString(", %1").arg(m_interface2->vlanId());

        m_commandList.append(QString("interface %1").arg(m_switchPort));
        m_commandList.append(QString("  switchport"));
        m_commandList.append(QString("  switchport mode trunk"));
        m_commandList.append(QString("  switchport trunk allowed vlan %1").arg(vlans));
        m_commandList.append(QString("  no shutdown"));
    }

    VMTDParticipant::EnRole VMTDParticipant::role() const
    {
        return m_role;
    }
    void VMTDParticipant::setRole(EnRole role)
    {
        m_role = role;
    }

    int VMTDParticipant::defaultVlanId() const
    {
        return m_defaultVlanId;
    }
    void VMTDParticipant::setDefaultVlanId(int defaultVlanId)
    {
        m_defaultVlanId = defaultVlanId;
    }

    QString VMTDParticipant::hostName() const
    {
        return m_hostName;
    }
    void VMTDParticipant::setHostName(const QString &hostName)
    {
        m_hostName = hostName;
    }

    QString VMTDParticipant::hostIp() const
    {
        return m_hostIp;
    }
    void VMTDParticipant::setHostIp(const QString &hostIp)
    {
        m_hostIp = hostIp;
    }

    QString VMTDParticipant::hostInterface() const
    {
        return m_hostInterface;
    }
    void VMTDParticipant::setHostInterface(const QString &hostInterface)
    {
        m_hostInterface = hostInterface;
    }

    QString VMTDParticipant::switchName() const
    {
        return m_switchName;
    }
    void VMTDParticipant::setSwitchName(const QString &switchName)
    {
        m_switchName = switchName;
    }

    QString VMTDParticipant::switchUrl() const
    {
        return m_switchUrl;
    }
    void VMTDParticipant::setSwitchUrl(const QString &switchUrl)
    {
        m_switchUrl = switchUrl;
    }

    QString VMTDParticipant::switchPort() const
    {
        return m_switchPort;
    }
    void VMTDParticipant::setSwitchPort(const QString &switchPort)
    {
        m_switchPort = switchPort;
    }

    VMTDInterface *VMTDParticipant::interface1() const
    {
        return m_interface1;
    }
    VMTDInterface *VMTDParticipant::interface2() const
    {
        return m_interface2;
    }

    const RequestList &VMTDParticipant::requests() const
    {
        return m_requestList;
    }
    const CommandList &VMTDParticipant::commands() const
    {
        return m_commandList;
    }
}
