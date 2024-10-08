#pragma once

#include "../VMTDSettings.h"
#include "../engine/VMTDParticipant.h"

#include <QtSql>

namespace VMTDLib
{
    class VMTDDeviceManagerForm;

    class VMTDDeviceManager : public QObject
    {
        Q_OBJECT

    public:

        VMTDDeviceManager(QObject *parent, VMTDSettings *settings);
        ~VMTDDeviceManager();

        VMTDSettings *settings() const;

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        bool              isDbOpened() const;
        const QSqlDatabase &db() const;
        QString             dbFileName() const;

        QString filePath() const;

        void selectSwitches();
        void createSwitch(int id, const QString &name,
                          const QString &url,
                          const QString &username,
                          const QString &password);
        void removeSwitch(int id);

        void selectHosts();
        void createHost(int id, const QString &name,
                        const QString &domainName,
                        const QString &ip,
                        const QString &interface,
                        const QString &switchport,
                        int switchId);
        void removeHost(int id);

        const QList<VMTDParticipant *> &participants() const;
        void                       buildParticipants();

        const QList<QString> &hostIps() const;
        void             buildHostIps();

        const QList<QUrl> &switchUrls() const;
        void          buildSwitchUrls();

        void selectBannedScanners();
        void createBannedScanner(int id, int hostId, int remaining);
        void removeBannedScanner(int id);

        void selectAllowedScanners();
        void createAllowedScanner(int id, int hostId);
        void removeAllowedScanner(int id);

        void apply();

    signals:

        void restartSignal();

        void switchesUpdatedSignal(const QSqlQuery &query);

        void hostsUpdatedSignal(const QSqlQuery &query);

        void bannedScannersUpdatedSignal(const QSqlQuery &query);

        void allowedScannersUpdatedSignal(const QSqlQuery &query);

    public slots:

        void updateUrlOnlineSlot(const QString &url, bool isOnline);

        void updateIpOnlineSlot(const QString &ip, bool isOnline);

    private:

        void printDbConnections();

        void createTables();
        void createTableSwitches();
        void createTableHosts();
        void createTableBannedScanners();
        void createTableAllowedScanners();

        const QString EXTENSION = ".db";
        const QString DIR_NAME = "db";

        QPointer<VMTDDeviceManagerForm> m_form;

        VMTDSettings *m_settings;

        QSqlDatabase m_db;
        QString      m_dbName;
        QString      m_dbConnectionName;

        QMap<QString, bool> m_urlOnline;
        QMap<QString, bool>  m_ipOnline;

        QList<VMTDParticipant *> m_participants;

        QList<QString> m_hostIps;
        QList<QUrl> m_switchUrls;
    };
}
