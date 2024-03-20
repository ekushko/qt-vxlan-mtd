#pragma once

#include <QString>
#include <QMap>
#include <QList>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

namespace VMTDLib
{
    static const QString VLAN_HEADER_TEMPLATE =
    {
        "vlans:\n"
    };

    static const QString VLAN_TEMPLATE =
    {
        "  vlan.%1:\n"
        "    id: %1\n"
        "    link: %2\n"
        "    addresses: [%3/%4]\n"
        "    macaddress: %5\n"
    };

    static const QString ROUTE_HEADER_TEMPLATE =
    {
        "    routes:\n"
    };

    static const QString ROUTE_TEMPLATE =
    {
        "      - to: %1/%2\n"
        "        via: %3\n"
        "        metric: %4\n"
    };

    static const QString NETWORK_TEMPLATE =
    {
        "100.100.%1.%2"
    };

    static const QString MAC_TEMPLATE =
    {
        "aa:bb:cc:dd:%1:%2"
    };

#define MTH_SETUP_INTERFACE_1 VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_INTERFACE_1)
#define MTH_SETUP_INTERFACE_2 VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_INTERFACE_2)
#define MTH_SETUP_HOSTS VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_HOSTS)
#define MTH_CLEAR_INTERFACE_1 VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_INTERFACE_1)
#define MTH_CLEAR_INTERFACE_2 VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_INTERFACE_2)
#define MTH_CLEAR_HOSTS VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_HOSTS)
#define MTH_CHECK_CONNECTION VMTDMethod::methods().value(VMTDMethod::EnName::CHECK_CONNECTION)

#define PRM_INTERFACE VMTDMethod::params().value(VMTDMethod::EnParamName::INTERFACE)
#define PRM_MAC VMTDMethod::params().value(VMTDMethod::EnParamName::MAC)
#define PRM_IP VMTDMethod::params().value(VMTDMethod::EnParamName::IP)
#define PRM_MASK VMTDMethod::params().value(VMTDMethod::EnParamName::MASK)
#define PRM_GATEWAY VMTDMethod::params().value(VMTDMethod::EnParamName::GATEWAY)
#define PRM_VLAN_ID VMTDMethod::params().value(VMTDMethod::EnParamName::VLAN_ID)
#define PRM_NETWORK VMTDMethod::params().value(VMTDMethod::EnParamName::NETWORK)
#define PRM_DOMAIN_NAME VMTDMethod::params().value(VMTDMethod::EnParamName::DOMAIN_NAME)
#define PRM_METRIC VMTDMethod::params().value(VMTDMethod::EnParamName::METRIC)
#define PRM_ROUTES VMTDMethod::params().value(VMTDMethod::EnParamName::ROUTES)
#define PRM_HOSTS VMTDMethod::params().value(VMTDMethod::EnParamName::HOSTS)

    class VMTDMethod
    {
    public:

        VMTDMethod() = delete;

        enum class EnName
        {
            SETUP_INTERFACE_1,
            SETUP_INTERFACE_2,
            SETUP_HOSTS,

            CLEAR_INTERFACE_1,
            CLEAR_INTERFACE_2,
            CLEAR_HOSTS,

            CHECK_CONNECTION
        };

        enum class EnParamName
        {
            INTERFACE,
            MAC,
            IP,
            MASK,
            GATEWAY,
            VLAN_ID,
            NETWORK,
            DOMAIN_NAME,
            METRIC,
            ROUTES,
            HOSTS
        };

        static const QMap<EnName, QString> &methods();

        static const QMap<EnParamName, QString> &params();

        static const QList<EnParamName> &methodParams(EnName methodName);

        static const QList<EnParamName> &subParams(EnParamName paramName);

        static bool checkParam(EnParamName paramName, const QJsonValue &jsonVal);
    };
}
