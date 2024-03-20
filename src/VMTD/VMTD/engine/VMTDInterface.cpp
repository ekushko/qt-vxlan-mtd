#include "VMTDInterface.h"

#include "../VMTDMethod.h"

namespace VMTDLib
{
    VMTDInterface::VMTDInterface()
    {
        m_groupIndex = -1;
        m_mac = "aa:bb:cc:dd:ee:ff";
        m_ip = QString(NETWORK_TEMPLATE).arg(0).arg(1);
        m_mask = 16;
        m_vlanId = 3;
    }

    int VMTDInterface::groupIndex()
    {
        return m_groupIndex;
    }
    void VMTDInterface::setGroupIndex(int groupIndex)
    {
        m_groupIndex = groupIndex;
    }

    QString VMTDInterface::mac() const
    {
        return m_mac;
    }
    void VMTDInterface::setMac(const QString &mac)
    {
        m_mac = mac;
    }

    QString VMTDInterface::ip() const
    {
        return m_ip;
    }
    void VMTDInterface::setIp(const QString &ip)
    {
        m_ip = ip;
    }

    uchar VMTDInterface::mask() const
    {
        return m_mask;
    }
    void VMTDInterface::setMask(uchar mask)
    {
        m_mask = mask;
    }

    int VMTDInterface::vlanId() const
    {
        return m_vlanId;
    }
    void VMTDInterface::setVlanId(int vlanId)
    {
        m_vlanId = vlanId;
    }

    const QList<VMTDRoute> &VMTDInterface::routes() const
    {
        return m_routes;
    }
    void VMTDInterface::clearRoutes()
    {
        m_routes.clear();
    }
    void VMTDInterface::addRoute(const QString &network, uchar mask,
                                 const QString &gateway, int metric)
    {
        m_routes.append(VMTDRoute(network, mask,
                                  gateway, metric));
    }
}
