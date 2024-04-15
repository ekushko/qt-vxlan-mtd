#include "VMTDGroup.h"

#include "../VMTDVarname.h"
#include "../VMTDMethod.h"

#include <QRandomGenerator>

namespace VMTDLib
{
    VMTDGroup::VMTDGroup(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_index = -1;
        m_maxParticipantCount = 2;
        m_octet3 = 1;
        m_mask = 24;
        m_vlanId = 1;
    }

    const QVector<VMTDParticipant *> &VMTDGroup::participants() const
    {
        return m_participants;
    }
    bool VMTDGroup::canAddParticipant() const
    {
        return m_participants.size() < maxParticipantCount();
    }
    void VMTDGroup::addParticipant(VMTDParticipant *participant)
    {
        if (!canAddParticipant())
        {
            m_settings->debugOut(QString("%1 | Group-%2 is crowded! Participant %3 not added!")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(index())
                                 .arg(participant->hostName()));
            return;
        }

        if (m_participants.contains(participant))
        {
            m_settings->debugOut(QString("%1 | Participant %2 is already in group-%3! ")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(participant->hostName())
                                 .arg(index()));
            return;
        }

        int octet4 = 0;

        if (m_settings->shouldRandomizeNetwork())
        {
            do
            {
                octet4 = QRandomGenerator::global()->bounded(1, 253);
            }
            while (m_octets4.contains(octet4));
        }
        else
        {
            octet4 = m_participants.size() + 1;
        }

        m_octets4.insert(octet4);

        participant->interface1()->setExist(true);
        participant->interface1()->setIp(QString(NETWORK_TEMPLATE).arg(m_octet3).arg(octet4));
        participant->interface1()->setMac(QString(MAC_TEMPLATE)
                                          .arg(QString("%1").arg(m_octet3, 2, 16, QLatin1Char('0')))
                                          .arg(QString("%1").arg(octet4, 2, 16, QLatin1Char('0'))));
        participant->interface1()->setGroupIndex(m_index);
        participant->interface1()->setMask(m_mask);
        participant->interface1()->setVlanId(m_vlanId);
        m_participants.append(participant);

        m_settings->debugOut(QString("%1 | Participant %2 was added in group-%3!")
                             .arg(VN_S(VMTDGroup))
                             .arg(participant->hostName())
                             .arg(index()));
    }
    void VMTDGroup::clearParticipants()
    {
        m_participants.clear();
    }

    VMTDParticipant *VMTDGroup::internalGateway() const
    {
        return m_internalGateway;
    }
    void VMTDGroup::setInternalGateway(VMTDParticipant *internalGateway)
    {
        if (internalGateway->interface1()->groupIndex() != index())
        {
            m_settings->debugOut(QString("%1 | Internal gateway should be in group-%2!")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(index()));
            return;
        }

        m_internalGateway = internalGateway;

        for (auto participant : m_participants)
        {
            if (participant != m_internalGateway)
                participant->interface1()->addRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                                                    internalGateway->interface1()->ip(), 100);
        }

        m_settings->debugOut(QString("%1 | Internal gateway %2 was setted in group-%3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(m_internalGateway->hostName())
                             .arg(index()));
    }

    VMTDParticipant *VMTDGroup::externalGateway() const
    {
        return m_externalGateway;
    }
    void VMTDGroup::setExternalGateway(VMTDParticipant *externalGateway)
    {
        if (externalGateway->interface1()->groupIndex() == m_index)
        {
            m_settings->debugOut(QString("%1 | Internal gateway should NOT be in group-%2!")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(index()));
            return;
        }

        m_externalGateway = externalGateway;

        int octet4 = 0;

        if (m_settings->shouldRandomizeGateway())
        {
            do
            {
                octet4 = QRandomGenerator::global()->bounded(1, 253);
            }
            while (m_octets4.contains(octet4));
        }
        else
        {
            octet4 = m_participants.size() + 1;
        }

        m_octets4.insert(octet4);

        m_externalGateway->interface2()->setExist(true);
        m_externalGateway->interface2()->setIp(QString(NETWORK_TEMPLATE)
                                               .arg(m_octet3)
                                               .arg(octet4));
        m_externalGateway->interface2()->setMac(QString(MAC_TEMPLATE)
                                                .arg(QString("%1").arg(m_octet3, 2, 16, QLatin1Char('0')))
                                                .arg(QString("%1").arg(octet4, 2, 16, QLatin1Char('0'))));
        m_externalGateway->interface2()->setGroupIndex(m_index);
        m_externalGateway->interface2()->setMask(m_mask);
        m_externalGateway->interface2()->setVlanId(m_vlanId);

        m_internalGateway->interface1()->addRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                                                  m_externalGateway->interface2()->ip(), 100);

        m_participants.append(m_externalGateway);

        m_settings->debugOut(QString("%1 | External gateway %2 was added in group-%3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(m_externalGateway->hostName())
                             .arg(index()));
    }

    int VMTDGroup::index() const
    {
        return m_index;
    }
    void VMTDGroup::setIndex(int index)
    {
        m_index = index;
    }

    int VMTDGroup::maxParticipantCount() const
    {
        return m_maxParticipantCount;
    }
    void VMTDGroup::setMaxParticipantCount(int maxParticipantCount)
    {
        m_maxParticipantCount = maxParticipantCount;
    }

    QString VMTDGroup::network() const
    {
        return QString(NETWORK_TEMPLATE).arg(m_octet3).arg(0);
    }

    int VMTDGroup::octet3() const
    {
        return m_octet3;
    }
    void VMTDGroup::setOctet3(int octet3)
    {
        m_octet3 = octet3;
    }

    uchar VMTDGroup::mask() const
    {
        return m_mask;
    }
    void VMTDGroup::setMask(uchar mask)
    {
        m_mask = mask;
    }

    int VMTDGroup::vlanId() const
    {
        return m_vlanId;
    }
    void VMTDGroup::setVlanId(int vlanId)
    {
        m_vlanId = vlanId;
    }
}
