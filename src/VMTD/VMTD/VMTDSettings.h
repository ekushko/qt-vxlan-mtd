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

        //

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

        QString m_dbName;
        bool m_shouldCheckOnline;
        int m_defaultVlanId;
        int m_minVlanId;
        int m_maxVlanId;

        bool m_shouldBeRestarted;

    private slots:

        void saveSlot();
        void loadSlot();
    };
}
