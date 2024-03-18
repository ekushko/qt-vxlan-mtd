#pragma once

#include "../VMTDSettings.h"

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

        void showForm();

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
                        const QString &ip,
                        const QString &interface,
                        const QString &switchport,
                        int switchId);
        void removeHost(int id);

    signals:

        void switchesUpdatedSignal(const QSqlQuery &query);

        void hostsUpdatedSignal(const QSqlQuery &query);

    private:

        void printDbConnections();

        void createTables();
        void createTableSwitches();
        void createTableHosts();

        const QString EXTENSION = ".db";
        const QString DIR_NAME = "db";

        QPointer<VMTDDeviceManagerForm> m_form;

        VMTDSettings *m_settings;

        QSqlDatabase m_db;
        QString      m_dbName;
        QString      m_dbConnectionName;
    };
}
