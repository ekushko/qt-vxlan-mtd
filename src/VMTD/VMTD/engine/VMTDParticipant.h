#pragma once

#include "VMTDInterface.h"
#include "../VMTDAlias.h"

#include <QObject>
#include <QJsonObject>

namespace VMTDLib
{
    class VMTDParticipant : public QObject
    {
        Q_OBJECT

    public:

        explicit VMTDParticipant(QObject *parent = nullptr);
        ~VMTDParticipant();

        // КОНСТАНТЫ

        enum class EnRole
        {
            ENDPOINT,
            GATEWAY
        };
        Q_ENUM(EnRole)
        static const QString            &enRoleToS(const EnRole &role);
        static const QMap<int, QString> &enRoleToL();

        // ЛОГИКА

        void buildRequests();
        void buildCommands();

        // ДАННЫЕ

        EnRole  role() const;
        void setRole(EnRole role);

        int     defaultVlanId() const;
        void setDefaultVlanId(int defaultVlanId);

        int     hostId() const;
        void setHostId(int hostId);

        QString hostName() const;
        void setHostName(const QString &hostName);

        QString hostDomainName() const;
        void setHostDomainName(const QString &hostDomainName);

        QString hostIp() const;
        void setHostIp(const QString &hostIp);

        QString hostInterface() const;
        void setHostInterface(const QString &hostInterface);

        QString switchName() const;
        void setSwitchName(const QString &switchName);

        QString switchUrl() const;
        void setSwitchUrl(const QString &switchUrl);

        QString switchPort() const;
        void setSwitchPort(const QString &switchPort);

        VMTDInterface *interface1() const;
        VMTDInterface *interface2() const;

        const RequestList &requests() const;
        const CommandList &commands() const;

    private:

        int m_hostId;

        EnRole m_role;

        int m_defaultVlanId;

        QString m_hostName;
        QString m_hostDomainName;
        QString m_hostIp;
        QString m_hostInterface;

        QString m_switchName;
        QString m_switchUrl;
        QString m_switchPort;

        VMTDInterface *m_interface1;
        VMTDInterface *m_interface2;

        RequestList m_requestList;
        CommandList m_commandList;
    };
}
