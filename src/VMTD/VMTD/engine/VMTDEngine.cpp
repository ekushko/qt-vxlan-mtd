#include "VMTDEngine.h"
#include "VMTDEngineForm.h"

#include "../VMTDVarname.h"
#include "../VMTDMethod.h"

#include <QRandomGenerator>

namespace VMTDLib
{
    VMTDEngine::VMTDEngine(QObject *parent,
                           VMTDSettings *settings,
                           VMTDDeviceManager *manager)
        : QObject(parent)
        , m_settings(settings)
        , m_manager(manager)
    {

    }

    VMTDEngine::~VMTDEngine()
    {
        if (m_form != nullptr)
            delete m_form;
    }

    QWidget *VMTDEngine::form() const
    {
        return m_form;
    }
    void VMTDEngine::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDEngineForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDEngine::generate()
    {
        m_manager->buildParticipants();

        if (m_manager->participants().size() < 2)
        {
            m_settings->debugOut(QString("%1 | Participants are not enough!")
                                 .arg(VN_S(VMTDEngine)));
            return;
        }

        createGroups();
        createGateways();
    }

    void VMTDEngine::run()
    {
        if (m_manager->participants().size() < 2)
        {
            m_settings->debugOut(QString("%1 | Participants are not enough!")
                                 .arg(VN_S(VMTDEngine)));
            return;
        }

        createCommands();
        createRequests();
    }

    const QList<VMTDGroup *> &VMTDEngine::groups() const
    {
        return m_groups;
    }

    void VMTDEngine::createGroups()
    {
        qDeleteAll(m_groups);
        m_groups.clear();

        const auto &participants = m_manager->participants();

        const int groupCount = QRandomGenerator::global()->bounded(2, participants.size());

        m_settings->debugOut(QString("%1 | Count of groups: %2")
                             .arg(VN_S(VMTDEngine))
                             .arg(groupCount));

        QSet<int> _vlanIds, _octets3;

        for (int i = 0; i < groupCount; ++i)
        {
            auto group = new VMTDGroup(this, m_settings);

            // индекс

            group->setIndex(i);

            // vlan id

            int vlanId = 0;

            if (m_settings->shouldRandomizeVlan())
            {
                do
                {
                    vlanId = QRandomGenerator::global()->bounded(m_settings->minVlanId(), m_settings->maxVlanId());
                    group->setVlanId(vlanId);
                }
                while (_vlanIds.contains(vlanId));
            }
            else
            {
                vlanId = m_settings->minVlanId() + i;
                group->setVlanId(vlanId);
            }

            _vlanIds.insert(vlanId);

            // сеть

            int octet3 = 0;

            if (m_settings->shouldRandomizeNetwork())
            {
                do
                {
                    octet3 = QRandomGenerator::global()->bounded(1, 253);
                    group->setOctet3(octet3);
                }
                while (_octets3.contains(octet3));
            }
            else
            {
                group->setOctet3(vlanId);
            }

            _octets3.insert(octet3);

            // маска

            group->setMask(24);

            // максимальное число участников

            const auto whole = participants.size() / groupCount;
            const auto remainder = participants.size() % groupCount;

            if (i == 0 && remainder != 0)
                group->setMaxParticipantCount(whole + remainder);
            else
                group->setMaxParticipantCount(whole);

            m_groups.append(group);
        }

        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        QList<VMTDParticipant *> _participants(participants.begin(), participants.end());
        QList<VMTDGroup *> _groups(m_groups.begin(), m_groups.end());

        if (m_settings->shouldRandomizeParticipant())
        {
            std::shuffle(_participants.begin(), _participants.end(), generator);
            std::shuffle(_groups.begin(), _groups.end(), generator);
        }

        for (int i = 0, j = 0; i < _groups.size(); ++i)
        {
            auto group = _groups.at(i);

            while (group->canAddParticipant())
            {
                auto participant = _participants.at(j++);
                participant->setRole(VMTDParticipant::EnRole::ENDPOINT);

                group->addParticipant(participant);
            }
        }
    }

    void VMTDEngine::createGateways()
    {
        QList<VMTDParticipant *> _gateways;

        for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
        {
            auto group = *it;

            const auto index = qrand() % group->participants().size();

            auto gateway = group->participants().at(index);
            gateway->setRole(VMTDParticipant::EnRole::GATEWAY);
            _gateways.append(gateway);

            m_settings->debugOut(QString("%1 | Participant %2 was choosen as gateway in group-%3")
                                 .arg(VN_S(VMTDEngine))
                                 .arg(gateway->hostName())
                                 .arg(group->index()));
        }

        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        if (m_settings->shouldRandomizeGateway())
            std::shuffle(_gateways.begin(), _gateways.end(), generator);

        for (auto it = _gateways.begin(); it != _gateways.end(); ++it)
        {
            auto internalGateway = *it;
            auto group = m_groups.at(internalGateway->interface1()->groupIndex());
            group->setInternalGateway(internalGateway);

            const auto distance = std::distance(it, _gateways.end());

            if (distance <= 1)
                break;

            auto externalGateway = *std::next(it);
            group->setExternalGateway(externalGateway);

            if (distance <= 2)
                break;

            auto jt = it;
            std::advance(jt, 2);

            while (jt != _gateways.end())
            {
                auto remoteParticipant = *jt++;
                auto remoteGroup = m_groups.at(remoteParticipant->interface1()->groupIndex());

                internalGateway->interface1()->addRoute(remoteGroup->network(), remoteGroup->mask(),
                                                        externalGateway->interface2()->ip(), 100);
            }
        }

        for (auto it = _gateways.rbegin(); it != _gateways.rend(); ++it)
        {
            auto internalGateway = *it;

            const auto distance = std::distance(it, _gateways.rend());

            if (distance <= 1)
                break;

            auto externalGateway = *std::next(it);

            if (distance <= 2)
                break;

            auto jt = it;
            std::advance(jt, 2);

            while (jt != _gateways.rend())
            {
                auto remoteParticipant = *jt++;
                auto remoteGroup = m_groups.at(remoteParticipant->interface1()->groupIndex());

                if (internalGateway->interface2()->isExist())
                    internalGateway->interface2()->addRoute(remoteGroup->network(), remoteGroup->mask(),
                                                            externalGateway->interface1()->ip(), 100);
            }
        }
    }

    void VMTDEngine::createRequests()
    {
        for (auto participant : m_manager->participants())
        {
            participant->buildRequests();

            auto requests = participant->requests();
            requests.append(qMakePair(MTH_SETUP_HOSTS, hosts()));
            requests.append(qMakePair(MTH_APPLY_NETPLAN, QJsonObject()));

            emit appendRequestListSignal(participant->hostIp(), requests);
        }
    }

    void VMTDEngine::createCommands()
    {
        QSet<QString> _urls;

        for (auto participant : m_manager->participants())
            _urls.insert(participant->switchUrl());

        for (const auto &url : _urls)
        {
            CommandList commands;
            commands.append("conf t");

            for (auto participant : m_manager->participants())
            {
                if (participant->switchUrl() == url)
                {
                    participant->buildCommands();

                    commands.append(participant->commands());
                }
            }

            emit appendCommandListSignal(url, commands);
        }
    }

    QJsonObject VMTDEngine::hosts() const
    {
        QJsonObject jsonObj;
        QJsonArray jsonArr;

        for (auto participant : m_manager->participants())
        {
            QJsonObject itemObj;
            itemObj[PRM_IP] = participant->interface1()->ip();
            itemObj[PRM_DOMAIN_NAME] = participant->hostDomainName();
            jsonArr.append(itemObj);
        }

        jsonObj[PRM_HOSTS] = jsonArr;

        return jsonObj;
    }
}
