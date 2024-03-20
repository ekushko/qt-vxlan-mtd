#pragma once

#include <QString>

namespace VMTDLib
{
    class VMTDRoute
    {
    public:

        VMTDRoute();
        VMTDRoute(const QString &network, uchar mask,
                  const QString &gateway, int metric);

        // ДАННЫЕ

        QString network() const;
        void setNetwork(const QString &network);

        uchar   mask() const;
        void setMask(uchar mask);

        QString gateway() const;
        void setGateway(const QString &gateway);

        int     metric() const;
        void setMetric(int metric);

    private:

        QString m_network;
        uchar m_mask;
        QString m_gateway;
        int m_metric;
    };
}
