#pragma once

#include "VMTDDeclareShared.h"

#include <QObject>
#include <QPointer>

namespace VMTDLib
{
#define VMTDNodeType VMTDSettings::EnNodeType

    class VMTDSettingsForm;

    class VMTD_SHARED VMTDSettings : public QObject
    {
        Q_OBJECT

    public:

        VMTDSettings(QObject *parent, const QString &systemName);
        ~VMTDSettings();

        void showForm();

        // КОНСТАНТЫ

        enum class EnNodeType
        {
            CLIENT = 0,
            SERVER
        };
        Q_ENUM(EnNodeType)
        static const QString            &enNodeTypeToS(const EnNodeType &nodeType);
        static const QMap<int, QString> &enNodeTypeToL();

        // ЛОГИКА

        void debugOut(const QString &text);

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        QString filePath() const;

        void save();
        void load();

        void apply();

        // СИСТЕМНЫЕ

        EnNodeType nodeType() const;
        void    setNodeType(const EnNodeType &nodeType);

        QString systemName() const;

        QString debugName() const;
        void setDebugName(const QString &debugName);

        bool    shouldShowDebug() const;
        void setShouldShowDebug(bool shouldShowDebug);

        // ПАРАМЕТРЫ КОНФИГУРИРОВАНИЯ

        bool    shouldUseReconfigTimer() const;
        void setShouldUseReconfigTimer(bool shouldUseReconfigTimer);

        bool    shouldCalcReconfigInterval() const;
        void setShouldCalcReconfigInterval(bool shouldCalcReconfigInterval);

        int     reconfigInterval() const;
        void setReconfigInterval(int reconfigInterval);

        QString dbName() const;
        void setDbName(const QString &dbName);

        bool    shouldCheckOnline() const;
        void setShouldCheckOnline(bool shouldCheckOnline);

        int     defaultVlanId() const;
        void setDefaultVlanId(int defaultVlanId);

        int     minVlanId() const;
        void setMinVlanId(int minVlanId);

        int     maxVlanId() const;
        void setMaxVlanId(int maxVlanId);

        QString netplan1FileName() const;
        void setNetplan1FileName(const QString &netplan1FileName);

        QString netplan2FileName() const;
        void setNetplan2FileName(const QString &netplan2FileName);

        // СЕТЕВЫЕ ПАРАМЕТРЫ

        int     localPort() const;
        void setLocalPort(int localPort);

        QString serverIp() const;
        void setServerIp(const QString &serverIp);

        int     serverPort() const;
        void setServerPort(int serverPort);

        bool    shouldReconnect() const;
        void setShouldReconnect(bool shouldReconnect);

        int     reconnectInterval() const;
        void setReconnectInterval(int reconnectInterval);

        // ПАРАМЕТРЫ ПРОТОКОЛА

        bool    shouldCheckConnection() const;
        void setShouldCheckConnection(bool shouldCheckConnection);

        int     checkConnectionInterval() const;
        void setCheckConnectionInterval(int checkConnectionInterval);

        int     ticketTimeoutInterval() const;
        void setTicketTimeoutInterval(int ticketTimeoutInterval);

        int     checkQueueInterval() const;
        void setCheckQueueInterval(int checkQueueInterval);

    signals:

        void restartSignal();

        void saveSignal();
        void loadSignal();

    private:

        const QString DIR_NAME = "settings";
        const QString FILE_EXTENSION = ".settings";

        QPointer<VMTDSettingsForm> m_form;

        EnNodeType m_nodeType;
        const QString m_systemName;
        QString m_debugName;
        bool m_shouldShowDebug;

        bool m_shouldUseReconfigTimer;
        bool m_shouldCalcReconfigInterval;
        int m_reconfigInterval;
        QString m_dbName;
        bool m_shouldCheckOnline;
        int m_defaultVlanId;
        int m_minVlanId;
        int m_maxVlanId;
        QString m_netplan1FileName;
        QString m_netplan2FileName;

        int m_localPort;
        QString m_serverIp;
        int     m_serverPort;
        bool m_shouldReconnect;
        int m_reconnectInterval;

        bool m_shouldCheckConnection;
        int m_checkConnectionInterval;
        int m_ticketTimeoutInterval;
        int m_checkQueueInterval;

        bool m_shouldBeRestarted;

    private slots:

        void saveSlot();
        void loadSlot();
    };
}
