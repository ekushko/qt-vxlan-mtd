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

        void showForm();

        VMTDSettings *settings() const;

        const QString &netplan1() const;
        const QString &netplan2() const;
        const QString &hosts() const;

    public slots:

        void handleMethodSlot(const QString &method, const QJsonObject &params, bool &result);

        void showFormSlot();

    private:

        bool handleSetupInterface1(const QJsonObject &params);
        bool handleSetupInterface2(const QJsonObject &params);
        bool handleSetupHosts(const QJsonObject &params);

        bool handleClearInterface1(const QJsonObject &params);
        bool handleClearInterface2(const QJsonObject &params);
        bool handleClearRoutes(const QJsonObject &params);
        bool handleClearHosts(const QJsonObject &params);

        bool handleCheckConnection(const QJsonObject &params);

        QPointer<VMTDConfiguratorForm> m_form;

        VMTDSettings *m_settings;

        QString m_netplan1;
        QString m_netplan2;
        QString m_hosts;
    };
}
