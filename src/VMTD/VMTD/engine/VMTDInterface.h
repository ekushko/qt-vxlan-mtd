#pragma once

#include "VMTDRoute.h"

#include <QList>

namespace VMTDLib
{
    class VMTDInterface
    {
    public:

        VMTDInterface();

        // МЕТАДАННЫЕ

        bool  isExist() const;
        void setExist(bool isExist);

        int     groupIndex();
        void setGroupIndex(int groupIndex);

        // ДАННЫЕ

        QString mac() const;
        void setMac(const QString &mac);

        QString ip() const;
        void setIp(const QString &ip);

        uchar   mask() const;
        void setMask(uchar mask);

        int     vlanId() const;
        void setVlanId(int vlanId);

        const QList<VMTDRoute> &routes() const;
        void                clearRoutes();
        void                  addRoute(const QString &network, uchar mask,
                                       const QString &gateway, int metric);

    private:

        bool m_isExist;
        int m_groupIndex;
        QString m_mac;
        QString m_ip;
        uchar m_mask;
        int m_vlanId;

        QList<VMTDRoute> m_routes;
    };
}
