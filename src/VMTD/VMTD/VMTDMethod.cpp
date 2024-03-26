#include "VMTDMethod.h"

namespace VMTDLib
{
    const QMap<VMTDMethod::EnName, QString> &VMTDMethod::methods()
    {
        static const QMap<EnName, QString> map
        {
            { EnName::SETUP_INTERFACE_1,      "setupInterface1" },
            { EnName::SETUP_INTERFACE_2,      "setupInterface2" },
            { EnName::SETUP_HOSTS,            "setupHosts" },
            { EnName::CLEAR_INTERFACE_1,      "clearInterface1" },
            { EnName::CLEAR_INTERFACE_2,      "clearInterface2" },
            { EnName::CLEAR_HOSTS,            "clearHosts" },
            { EnName::APPLY_NETPLAN,          "applyNetplan" },
            { EnName::CHECK_CONNECTION,       "checkConnection" }
        };
        return map;
    }

    const QMap<VMTDMethod::EnParamName, QString> &VMTDMethod::params()
    {
        static const QMap<EnParamName, QString> map
        {
            { EnParamName::INTERFACE,   "interface" },
            { EnParamName::MAC,         "mac" },
            { EnParamName::IP,          "ip" },
            { EnParamName::MASK,        "mask" },
            { EnParamName::GATEWAY,     "gateway" },
            { EnParamName::VLAN_ID,     "vlanId" },
            { EnParamName::NETWORK,     "network" },
            { EnParamName::DOMAIN_NAME, "domainName" },
            { EnParamName::ROUTES,      "routes" },
            { EnParamName::HOSTS,       "hosts" }
        };
        return map;
    }

    const QList<VMTDMethod::EnParamName> &VMTDMethod::methodParams(EnName methodName)
    {
        if (methodName == EnName::SETUP_INTERFACE_1
            || methodName == EnName::SETUP_INTERFACE_2)
        {
            static const QList<EnParamName> list
            {
                EnParamName::INTERFACE,
                EnParamName::MAC,
                EnParamName::IP,
                EnParamName::MASK,
                EnParamName::VLAN_ID,
                EnParamName::ROUTES
            };
            return list;
        }
        else if (methodName == EnName::SETUP_HOSTS)
        {
            static const QList<EnParamName> list
            {
                EnParamName::HOSTS
            };
            return list;
        }

        static const QList<EnParamName> list
        {
            // nothing
        };
        return list;
    }

    const QList<VMTDMethod::EnParamName> &VMTDMethod::subParams(EnParamName paramName)
    {
        if (paramName == EnParamName::ROUTES)
        {
            static const QList<EnParamName> list
            {
                EnParamName::IP,
                EnParamName::MASK,
                EnParamName::GATEWAY,
                EnParamName::METRIC
            };
            return list;
        }
        else if (paramName == EnParamName::HOSTS)
        {
            static const QList<EnParamName> list
            {
                EnParamName::IP,
                EnParamName::DOMAIN_NAME
            };
            return list;
        }

        static const QList<EnParamName> list
        {
            // nothing
        };
        return list;
    }

    bool VMTDMethod::checkParam(EnParamName paramName, const QJsonValue &jsonVal)
    {
        if (jsonVal.isUndefined())
            return false;
        else if (subParams(paramName).isEmpty() || (!jsonVal.isObject() && !jsonVal.isArray()))
            return true;

        const auto &subParamsList = VMTDMethod::subParams(paramName);

        for (auto subParamName : subParamsList)
        {
            const auto subParamNameStr = VMTDMethod::params().value(subParamName);

            if (jsonVal.isObject())
            {
                const auto jsonObj = jsonVal.toObject();

                if (!jsonObj.contains(subParamNameStr)
                    || !checkParam(subParamName, jsonObj[subParamNameStr]))
                    return false;
            }
            else if (jsonVal.isArray())
            {
                const auto jsonArr = jsonVal.toArray();

                for (const auto &jsonArrVal : jsonArr)
                {
                    if (!checkParam(subParamName, jsonArrVal))
                        return false;
                }
            }
        }

        return true;
    }
}
