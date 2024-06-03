#pragma once

#include "../VMTDSettings.h"
#include "VMTDWatchdog.h"

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

    signals:

        void applyNetplanSignal();

    public slots:

        void handleMethodSlot(const QString &method, const QJsonObject &params, QJsonValue &result);

    private:

        QJsonValue handleSetupInterface1(const QJsonObject &params);
        QJsonValue handleSetupInterface2(const QJsonObject &params);
        QJsonValue handleSetupHosts(const QJsonObject &params);

        QJsonValue handleClearInterface1(const QJsonObject &params);
        QJsonValue handleClearInterface2(const QJsonObject &params);
        QJsonValue handleClearHosts(const QJsonObject &params);

        QJsonValue handleGetScanners(const QJsonObject &params);

        QJsonValue handleApplyNetplan(const QJsonObject &params);

        QJsonValue handleCheckConnection(const QJsonObject &params);

        void    save(const QString &filePath, const QString &data);
        QString load(const QString &filePath);

        void    saveHosts(const QString &filePath, const QString &data);
        QString loadHosts(const QString &filePath);

        QPointer<VMTDConfiguratorForm> m_form;

        VMTDSettings *m_settings;
        VMTDWatchdog *m_watchdog;

        QString m_vlanInterface1;
        QString m_vlanInterface2;

        QString m_netplan1;
        QString m_netplan2;
        QString m_hosts;

    private slots:

        void applyNetplanSlot();
    };
}
