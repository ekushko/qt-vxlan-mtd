#pragma once

#include "../VMTDSettings.h"
#include "VMTDParticipant.h"

#include <QSet>

namespace VMTDLib
{
    class VMTDGroup : public QObject
    {
        Q_OBJECT

    public:

        VMTDGroup(QObject *parent, VMTDSettings *settings);

        // ЛОГИКА

        const QVector<VMTDParticipant *> &participants() const;
        bool canAddParticipant() const;
        void    addParticipant(VMTDParticipant *participant);
        void  clearParticipants();

        VMTDParticipant *internalGateway() const;
        void          setInternalGateway(VMTDParticipant *internalGateway);

        VMTDParticipant *externalGateway() const;
        void          setExternalGateway(VMTDParticipant *externalGateway);

        // МЕТАДАННЫЕ

        int     index() const;
        void setIndex(int index);

        int     maxParticipantCount() const;
        void setMaxParticipantCount(int maxParticipantCount);

        // ДАННЫЕ

        QString network() const;

        int     octet3() const;
        void setOctet3(int octet3);

        uchar   mask() const;
        void setMask(uchar mask);

        int     vlanId() const;
        void setVlanId(int vlanId);

    private:

        VMTDSettings *m_settings;

        int m_index;
        int m_maxParticipantCount;
        int m_octet3;
        uchar m_mask;
        int m_vlanId;

        QVector<VMTDParticipant *> m_participants;

        QSet<int> m_octets4;

        VMTDParticipant *m_internalGateway = nullptr;
        VMTDParticipant *m_externalGateway = nullptr;
    };
}
