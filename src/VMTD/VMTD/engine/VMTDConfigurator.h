#pragma once

#include "../VMTDSettings.h"

#include <QObject>

namespace VMTDLib
{
    class VMTDConfiguratorForm;

    class VMTDConfigurator : public QObject
    {
        Q_OBJECT

    public:

        VMTDConfigurator(QObject *parent, VMTDSettings *settings);
        ~VMTDConfigurator();

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        VMTDSettings *settings() const;

        const QString &netplan1() const;
        void        setNetplan1(const QString &netplan1);
        QString        netplan1FilePath() const;

        const QString &netplan2() const;
        void        setNetplan2(const QString &netplan2);
        QString        netplan2FilePath() const;

        const QString &hosts() const;
        void        setHosts(const QString &hosts);
        QString        hostsFilePath() const;

        bool applyNetplan();

    public slots:

        void handleMethodSlot(const QString &method, const QJsonObject &params, bool &result);

    private:

        bool handleSetupInterface1(const QJsonObject &params);
        bool handleSetupInterface2(const QJsonObject &params);
        bool handleSetupHosts(const QJsonObject &params);

        bool handleClearInterface1(const QJsonObject &params);
        bool handleClearInterface2(const QJsonObject &params);
        bool handleClearRoutes(const QJsonObject &params);
        bool handleClearHosts(const QJsonObject &params);

        bool handleApplyNetplan(const QJsonObject &params);

        bool handleCheckConnection(const QJsonObject &params);

        QPointer<VMTDConfiguratorForm> m_form;

        VMTDSettings *m_settings;

        QString m_netplan1;
        QString m_netplan2;
        QString m_hosts;
    };
}
