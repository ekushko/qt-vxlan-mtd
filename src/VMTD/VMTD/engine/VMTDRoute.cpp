#include "VMTDRoute.h"

#include "../VMTDMethod.h"

namespace VMTDLib
{
    VMTDRoute::VMTDRoute()
        : VMTDRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                    QString(NETWORK_TEMPLATE).arg(0).arg(1), 100)
    {

    }

    VMTDRoute::VMTDRoute(const QString &network, uchar mask,
                         const QString &gateway, int metric)
        : m_network(network)
        , m_mask(mask)
        , m_gateway(gateway)
        , m_metric(metric)
    {
        // do nothing
    }

    QString VMTDRoute::network() const
    {
        return m_network;
    }
    void VMTDRoute::setNetwork(const QString &network)
    {
        m_network = network;
    }

    uchar VMTDRoute::mask() const
    {
        return m_mask;
    }
    void VMTDRoute::setMask(uchar mask)
    {
        m_mask = mask;
    }

    QString VMTDRoute::gateway() const
    {
        return m_gateway;
    }
    void VMTDRoute::setGateway(const QString &gateway)
    {
        m_gateway = gateway;
    }

    int VMTDRoute::metric() const
    {
        return m_metric;
    }
    void VMTDRoute::setMetric(int metric)
    {
        m_metric = metric;
    }
}
