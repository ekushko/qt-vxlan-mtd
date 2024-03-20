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

        do
        {
            octet4 = QRandomGenerator::global()->bounded(1, 254);
            participant->interface1()->setIp(QString(NETWORK_TEMPLATE).arg(m_octet3).arg(octet4));
            participant->interface1()->setMac(QString(MAC_TEMPLATE)
                                              .arg(QString("%1").arg(m_octet3, 2, 16, QLatin1Char('0')))
                                              .arg(QString("%1").arg(octet4, 2, 16, QLatin1Char('0'))));
        }
        while (m_octets4.contains(octet4));

        m_octets4.insert(octet4);

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

    VMTDParticipant *VMTDGroup::gateway() const
    {
        return m_gateway;
    }
    void VMTDGroup::setGateway(VMTDParticipant *gateway)
    {
        int octet4 = 0;

        do
        {
            octet4 = QRandomGenerator::global()->bounded(1, 254);
            gateway->interface2()->setIp(QString(NETWORK_TEMPLATE)
                                         .arg(m_octet3)
                                         .arg(octet4));
            gateway->interface2()->setMac(QString(MAC_TEMPLATE)
                                          .arg(QString("%1").arg(m_octet3, 2, 16, QLatin1Char('0')))
                                          .arg(QString("%1").arg(octet4, 2, 16, QLatin1Char('0'))));
        }
        while (m_octets4.contains(octet4));

        m_octets4.insert(octet4);

        gateway->interface2()->setGroupIndex(m_index);
        gateway->interface2()->setMask(m_mask);
        gateway->interface2()->setVlanId(m_vlanId);

        for (int i = 0; i < m_participants.size(); ++i)
        {
            auto participant = m_participants.at(i);
            participant->interface1()->addRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                                                gateway->interface1()->ip(), 100);
        }

        m_settings->debugOut(QString("%1 | Gateway %2 was added in group-%3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(gateway->hostName())
                             .arg(index()));

        m_gateway = gateway;
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
