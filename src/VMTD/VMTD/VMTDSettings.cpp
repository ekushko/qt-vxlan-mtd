#include "VMTDSettings.h"
#include "VMTDSettingsForm.h"

#include "VMTDVarname.h"
#include "VMTDTemplate.h"

#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDSettings::VMTDSettings(QObject *parent, const QString &systemName)
        : QObject(parent)
        , m_systemName(systemName)
    {
        m_nodeType = EnNodeType::CLIENT;
        m_debugName = "VMTD";
        m_shouldShowDebug = true;

        m_dbName = "physical_description";
        m_shouldCheckOnline = false;
        m_shouldUseReconfigTimer = false;
        m_reconfigInterval = 60000;
        m_shouldRandomizeVlan = false;
        m_defaultVlanId = 19;
        m_minVlanId = 20;
        m_maxVlanId = 50;
        m_shouldRandomizeNetwork = false;
        m_shouldRandomizeGateway = false;
        m_shouldRandomizeParticipant = false;
        m_netplan1FileName = "02-vmtd-1";
        m_netplan2FileName = "03-vmtd-2";
        m_shouldControlAlert = true;
        m_alertFilePath = QString("%1var%1log%1snort%1alert")
                          .arg(QDir::separator());

        m_localPort = 30001;
        m_serverIp = "127.0.0.1";
        m_serverPort = 30000;
        m_shouldReconnect = false;
        m_reconnectInterval = 1000;

        m_shouldCheckConnection = false;
        m_checkConnectionInterval = 100;
        m_checkQueueInterval = 100;
        m_ticketTimeoutInterval = 3000;

        m_shouldBeRestarted = false;

        connect(this, &VMTDSettings::saveSignal, this, &VMTDSettings::saveSlot);
        connect(this, &VMTDSettings::loadSignal, this, &VMTDSettings::loadSlot);

        if (!QDir(DIR_NAME).exists())
        {
            if (!QDir().mkdir(DIR_NAME))
            {
                debugOut(VN_S(VMTDSettings) + " | DIR creating: could not create directory " + DIR_NAME);
                return;
            }
        }
        else
        {
            load();
        }
    }

    VMTDSettings::~VMTDSettings()
    {
        if (m_form != nullptr)
            delete m_form;
    }

    void VMTDSettings::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDSettingsForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    const QString &VMTDSettings::enNodeTypeToS(const EnNodeType &nodeType)
    {
        switch (nodeType)
        {
        // *INDENT-OFF*
        case EnNodeType::CLIENT : RETURN_S("Client");
        case EnNodeType::SERVER : RETURN_S("Server");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDSettings::enNodeTypeToL()
    {
        RETURN_MAP(EnNodeType, enNodeTypeToS);
    }

    void VMTDSettings::debugOut(const QString &text)
    {
        if (m_shouldShowDebug)
            qDebug() << m_debugName +  " | " + text;
    }

    QJsonObject VMTDSettings::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_nodeType)] = static_cast<int>(m_nodeType);
        jsonObj[VN_ME(m_systemName)] = m_systemName;
        jsonObj[VN_ME(m_debugName)] = m_debugName;
        jsonObj[VN_ME(m_shouldShowDebug)] = m_shouldShowDebug;

        jsonObj[VN_ME(m_dbName)] = m_dbName;
        jsonObj[VN_ME(m_shouldCheckOnline)] = m_shouldCheckOnline;
        jsonObj[VN_ME(m_shouldUseReconfigTimer)] = m_shouldUseReconfigTimer;
        jsonObj[VN_ME(m_reconfigInterval)] = m_reconfigInterval;
        jsonObj[VN_ME(m_shouldRandomizeVlan)] = m_shouldRandomizeVlan;
        jsonObj[VN_ME(m_defaultVlanId)] = m_defaultVlanId;
        jsonObj[VN_ME(m_minVlanId)] = m_minVlanId;
        jsonObj[VN_ME(m_maxVlanId)] = m_maxVlanId;
        jsonObj[VN_ME(m_shouldRandomizeNetwork)] = m_shouldRandomizeNetwork;
        jsonObj[VN_ME(m_shouldRandomizeGateway)] = m_shouldRandomizeGateway;
        jsonObj[VN_ME(m_shouldRandomizeParticipant)] = m_shouldRandomizeParticipant;
        jsonObj[VN_ME(m_netplan1FileName)] = m_netplan1FileName;
        jsonObj[VN_ME(m_netplan2FileName)] = m_netplan2FileName;
        jsonObj[VN_ME(m_shouldControlAlert)] = m_shouldControlAlert;
        jsonObj[VN_ME(m_alertFilePath)] = m_alertFilePath;

        jsonObj[VN_ME(m_localPort)] = m_localPort;
        jsonObj[VN_ME(m_serverIp)] = m_serverIp;
        jsonObj[VN_ME(m_serverPort)] = m_serverPort;
        jsonObj[VN_ME(m_shouldReconnect)] = m_shouldReconnect;
        jsonObj[VN_ME(m_reconnectInterval)] = m_reconnectInterval;

        jsonObj[VN_ME(m_shouldCheckConnection)] = m_shouldCheckConnection;
        jsonObj[VN_ME(m_checkConnectionInterval)] = m_checkConnectionInterval;
        jsonObj[VN_ME(m_checkQueueInterval)] = m_checkQueueInterval;
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;

        return jsonObj;
    }
    void VMTDSettings::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_nodeType = (EnNodeType)jsonObj[VN_ME(m_nodeType)].toInt((int)m_nodeType);
        m_debugName = jsonObj[VN_ME(m_debugName)].toString(m_debugName);
        m_shouldShowDebug = jsonObj[VN_ME(m_shouldShowDebug)].toBool(m_shouldShowDebug);

        m_dbName = jsonObj[VN_ME(m_dbName)].toString(m_dbName);
        m_shouldCheckOnline = jsonObj[VN_ME(m_shouldCheckOnline)].toBool(m_shouldCheckOnline);
        m_shouldUseReconfigTimer = jsonObj[VN_ME(m_shouldUseReconfigTimer)]
                                   .toBool(m_shouldUseReconfigTimer);
        m_reconfigInterval = jsonObj[VN_ME(m_reconfigInterval)].toInt(m_reconfigInterval);
        m_shouldRandomizeVlan = jsonObj[VN_ME(m_shouldRandomizeVlan)].toBool(m_shouldRandomizeVlan);
        m_defaultVlanId = jsonObj[VN_ME(m_defaultVlanId)].toInt(m_defaultVlanId);
        m_minVlanId = jsonObj[VN_ME(m_minVlanId)].toInt(m_minVlanId);
        m_maxVlanId = jsonObj[VN_ME(m_maxVlanId)].toInt(m_maxVlanId);
        m_shouldRandomizeNetwork = jsonObj[VN_ME(m_shouldRandomizeNetwork)]
                                   .toBool(m_shouldRandomizeNetwork);
        m_shouldRandomizeGateway = jsonObj[VN_ME(m_shouldRandomizeGateway)]
                                   .toBool(m_shouldRandomizeGateway);
        m_shouldRandomizeParticipant = jsonObj[VN_ME(m_shouldRandomizeParticipant)]
                                       .toBool(m_shouldRandomizeParticipant);
        m_netplan1FileName = jsonObj[VN_ME(m_netplan1FileName)].toString(m_netplan1FileName);
        m_netplan2FileName = jsonObj[VN_ME(m_netplan2FileName)].toString(m_netplan2FileName);
        m_shouldControlAlert = jsonObj[VN_ME(m_shouldControlAlert)].toBool(m_shouldControlAlert);
        m_alertFilePath = jsonObj[VN_ME(m_alertFilePath)].toString(m_alertFilePath);

        m_localPort = jsonObj[VN_ME(m_localPort)].toInt(m_localPort);
        m_serverIp = jsonObj[VN_ME(m_serverIp)].toString(m_serverIp);
        m_serverPort = jsonObj[VN_ME(m_serverPort)].toInt(m_serverPort);
        m_shouldReconnect = jsonObj[VN_ME(m_shouldReconnect)].toBool(m_shouldReconnect);
        m_reconnectInterval = jsonObj[VN_ME(m_reconnectInterval)].toInt(m_reconnectInterval);

        m_shouldCheckConnection = jsonObj[VN_ME(m_shouldCheckConnection)]
                                  .toBool(m_shouldCheckConnection);
        m_checkConnectionInterval = jsonObj[VN_ME(m_checkConnectionInterval)]
                                    .toInt(m_checkConnectionInterval);
        m_checkQueueInterval = jsonObj[VN_ME(m_checkQueueInterval)]
                               .toInt(m_checkQueueInterval);
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);
    }

    QString VMTDSettings::filePath() const
    {
        QString filePath = QString("%1%2%3%4")
                           .arg(DIR_NAME,
                                QDir::separator(),
                                m_systemName,
                                FILE_EXTENSION);

        return filePath;
    }

    void VMTDSettings::save()
    {
        emit saveSignal();
    }
    void VMTDSettings::load()
    {
        emit loadSignal();
    }

    void VMTDSettings::apply()
    {
        if (m_shouldBeRestarted)
        {
            emit restartSignal();

            m_shouldBeRestarted = false;
        }
    }

    VMTDSettings::EnNodeType VMTDSettings::nodeType() const
    {
        return m_nodeType;
    }
    void VMTDSettings::setNodeType(const VMTDSettings::EnNodeType &nodeType)
    {
        if (m_nodeType != nodeType)
        {
            m_nodeType = nodeType;

            m_shouldBeRestarted = true;
        }
    }

    QString VMTDSettings::systemName() const
    {
        return m_systemName;
    }

    QString VMTDSettings::debugName() const
    {
        return m_debugName;
    }
    void VMTDSettings::setDebugName(const QString &debugName)
    {
        m_debugName = debugName;
    }

    bool VMTDSettings::shouldShowDebug() const
    {
        return m_shouldShowDebug;
    }
    void VMTDSettings::setShouldShowDebug(bool shouldShowDebug)
    {
        m_shouldShowDebug = shouldShowDebug;
    }

    QString VMTDSettings::dbName() const
    {
        return m_dbName;
    }
    void VMTDSettings::setDbName(const QString &dbName)
    {
        m_dbName = dbName;
    }

    bool VMTDSettings::shouldCheckOnline() const
    {
        return m_shouldCheckOnline;
    }
    void VMTDSettings::setShouldCheckOnline(bool shouldCheckOnline)
    {
        m_shouldCheckOnline = shouldCheckOnline;
    }

    bool VMTDSettings::shouldUseReconfigTimer() const
    {
        return m_shouldUseReconfigTimer;
    }
    void VMTDSettings::setShouldUseReconfigTimer(bool shouldUseReconfigTimer)
    {
        if (m_shouldUseReconfigTimer != shouldUseReconfigTimer)
        {
            m_shouldUseReconfigTimer = shouldUseReconfigTimer;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::reconfigInterval() const
    {
        return m_reconfigInterval;
    }
    void VMTDSettings::setReconfigInterval(int reconfigInterval)
    {
        if (m_reconfigInterval != reconfigInterval)
        {
            m_reconfigInterval = reconfigInterval;

            m_shouldBeRestarted = true;
        }
    }

    bool VMTDSettings::shouldRandomizeVlan() const
    {
        return m_shouldRandomizeVlan;
    }
    void VMTDSettings::setShouldRandomizeVlan(bool shouldRandomizeVlan)
    {
        m_shouldRandomizeVlan = shouldRandomizeVlan;
    }

    int VMTDSettings::defaultVlanId() const
    {
        return m_defaultVlanId;
    }
    void VMTDSettings::setDefaultVlanId(int defaultVlanId)
    {
        m_defaultVlanId = defaultVlanId;
    }

    int VMTDSettings::minVlanId() const
    {
        return m_minVlanId;
    }
    void VMTDSettings::setMinVlanId(int minVlanId)
    {
        if (minVlanId > maxVlanId())
            return;

        m_minVlanId = minVlanId;
    }

    int VMTDSettings::maxVlanId() const
    {
        return m_maxVlanId;
    }
    void VMTDSettings::setMaxVlanId(int maxVlanId)
    {
        if (maxVlanId < minVlanId())
            return;

        m_maxVlanId = maxVlanId;
    }

    bool VMTDSettings::shouldRandomizeNetwork() const
    {
        return m_shouldRandomizeNetwork;
    }
    void VMTDSettings::setShouldRandomizeNetwork(bool shouldRandomizeNetwork)
    {
        m_shouldRandomizeNetwork = shouldRandomizeNetwork;
    }

    bool VMTDSettings::shouldRandomizeGateway() const
    {
        return m_shouldRandomizeGateway;
    }
    void VMTDSettings::setShouldRandomizeGateway(bool shouldRandomizeGateway)
    {
        m_shouldRandomizeGateway = shouldRandomizeGateway;
    }

    bool VMTDSettings::shouldRandomizeParticipant() const
    {
        return m_shouldRandomizeParticipant;
    }
    void VMTDSettings::setShouldRandomizeParticipant(bool shouldRandomizeParticipant)
    {
        m_shouldRandomizeParticipant = shouldRandomizeParticipant;
    }

    QString VMTDSettings::netplan1FileName() const
    {
        return m_netplan1FileName;
    }
    void VMTDSettings::setNetplan1FileName(const QString &netplan1FileName)
    {
        m_netplan1FileName = netplan1FileName;
    }

    QString VMTDSettings::netplan2FileName() const
    {
        return m_netplan2FileName;
    }
    void VMTDSettings::setNetplan2FileName(const QString &netplan2FileName)
    {
        m_netplan2FileName = netplan2FileName;
    }

    bool VMTDSettings::shouldControlAlert() const
    {
        return m_shouldControlAlert;
    }
    void VMTDSettings::setShouldControlAlert(bool shouldControlAlert)
    {
        if (m_shouldControlAlert != shouldControlAlert)
        {
            m_shouldControlAlert = shouldControlAlert;

            m_shouldBeRestarted = true;
        }
    }

    QString VMTDSettings::alertFilePath() const
    {
        return m_alertFilePath;
    }
    void VMTDSettings::setAlertFilePath(const QString &alertFilePath)
    {
        if (m_alertFilePath != alertFilePath)
        {
            m_alertFilePath = alertFilePath;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::localPort() const
    {
        return m_localPort;
    }
    void VMTDSettings::setLocalPort(int localPort)
    {
        if (m_localPort != localPort)
        {
            m_localPort = localPort;

            m_shouldBeRestarted = true;
        }
    }


    QString VMTDSettings::serverIp() const
    {
        return m_serverIp;
    }
    void VMTDSettings::setServerIp(const QString &serverIp)
    {
        if (m_serverIp != serverIp)
        {
            m_serverIp = serverIp;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::serverPort() const
    {
        return m_serverPort;
    }
    void VMTDSettings::setServerPort(int serverPort)
    {
        if (m_serverPort != serverPort)
        {
            m_serverPort = serverPort;

            m_shouldBeRestarted = true;
        }
    }

    bool VMTDSettings::shouldReconnect() const
    {
        return m_shouldReconnect;
    }
    void VMTDSettings::setShouldReconnect(bool shouldReconnect)
    {
        if (m_shouldReconnect != shouldReconnect)
        {
            m_shouldReconnect = shouldReconnect;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::reconnectInterval() const
    {
        return m_reconnectInterval;
    }
    void VMTDSettings::setReconnectInterval(int reconnectInterval)
    {
        if (m_reconnectInterval != reconnectInterval)
        {
            m_reconnectInterval = reconnectInterval;

            m_shouldBeRestarted = true;
        }
    }


    bool VMTDSettings::shouldCheckConnection() const
    {
        return m_shouldCheckConnection;
    }
    void VMTDSettings::setShouldCheckConnection(bool shouldCheckConnection)
    {
        m_shouldCheckConnection = shouldCheckConnection;
    }

    int VMTDSettings::checkConnectionInterval() const
    {
        return m_checkConnectionInterval;
    }
    void VMTDSettings::setCheckConnectionInterval(int checkConnectionInterval)
    {
        if (m_checkConnectionInterval != checkConnectionInterval)
        {
            m_checkConnectionInterval = checkConnectionInterval;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDSettings::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        if (m_ticketTimeoutInterval != ticketTimeoutInterval)
        {
            m_ticketTimeoutInterval = ticketTimeoutInterval;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::checkQueueInterval() const
    {
        return m_checkQueueInterval;
    }
    void VMTDSettings::setCheckQueueInterval(int checkQueueInterval)
    {
        if (m_checkQueueInterval != checkQueueInterval)
        {
            m_checkQueueInterval = checkQueueInterval;

            m_shouldBeRestarted = true;
        }
    }


    void VMTDSettings::saveSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.open(QIODevice::WriteOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Save: could not open settings file");
        }
        else
        {
            QJsonDocument jsonDoc;
            jsonDoc.setObject(toJson());
            settingsFile.write(jsonDoc.toJson());
            settingsFile.close();

            debugOut(VN_S(VMTDSettings) + " | Settings were saved successfully.");
        }
    }
    void VMTDSettings::loadSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.exists())
        {
            saveSlot();
            return;
        }

        if (!settingsFile.open(QIODevice::ReadOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Load: could not open settings file");
        }
        else
        {
            QByteArray settingsData = settingsFile.readAll();
            settingsFile.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(settingsData);
            fromJson(jsonDoc.object());

            debugOut(VN_S(VMTDSettings) + " | Settings were loaded successfully.");
        }
    }
}
