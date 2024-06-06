#include "VMTDDeviceManager.h"
#include "VMTDDeviceManagerForm.h"

#include "../VMTDVarname.h"

namespace VMTDLib
{
    VMTDDeviceManager::VMTDDeviceManager(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        if (!QDir(DIR_NAME).exists())
        {
            if (!QDir().mkdir(DIR_NAME))
            {
                m_settings->debugOut(VN_S(VMTDDeviceManager)
                                     + " | DIR creating: could not create directory "
                                     + DIR_NAME);
            }
        }

        m_dbConnectionName = m_settings->dbName() + "_connection";

        m_db = QSqlDatabase::addDatabase("QSQLITE", m_dbConnectionName);
        m_db.setDatabaseName(filePath());
        m_db.open();

        printDbConnections();

        createTables();
    }

    VMTDDeviceManager::~VMTDDeviceManager()
    {
        if (m_form != nullptr)
            delete m_form;

        m_db.close();
        m_db = QSqlDatabase();
        m_db.removeDatabase(m_dbConnectionName);

        printDbConnections();
    }

    VMTDSettings *VMTDDeviceManager::settings() const
    {
        return m_settings;
    }

    QWidget *VMTDDeviceManager::form() const
    {
        return m_form;
    }
    void VMTDDeviceManager::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDDeviceManagerForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    bool VMTDDeviceManager::isDbOpened() const
    {
        return m_db.isOpen();
    }

    const QSqlDatabase &VMTDDeviceManager::db() const
    {
        return m_db;
    }

    QString VMTDDeviceManager::dbFileName() const
    {
        return m_settings->dbName() + EXTENSION;
    }

    QString VMTDDeviceManager::filePath() const
    {
        return QString("%1%2%3")
               .arg(DIR_NAME)
               .arg(QDir::separator())
               .arg(dbFileName());
    }

    void VMTDDeviceManager::selectSwitches()
    {
        const auto queryStr =
            QString("SELECT * FROM Switches;");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Switches\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
        else
        {
            emit switchesUpdatedSignal(query);
        }
    }
    void VMTDDeviceManager::createSwitch(int id, const QString &name,
                                         const QString &url,
                                         const QString &username,
                                         const QString &password)
    {
        if (id == 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of switch!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        QString queryStr;

        if (id > 0)
        {
            queryStr =
                QString("UPDATE Switches SET name = '%1', url = '%2', username = '%3', password = '%4'"
                        "WHERE id = '%5';")
                .arg(name)
                .arg(url)
                .arg(username)
                .arg(password)
                .arg(id);
        }
        else if (id < 0)
        {
            queryStr =
                QString("INSERT INTO Switches (name, url, username, password) "
                        "VALUES ('%1', '%2', '%3', '%4');")
                .arg(name)
                .arg(url)
                .arg(username)
                .arg(password);
        }

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Switches\" inserting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }
    void VMTDDeviceManager::removeSwitch(int id)
    {
        if (id <= 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of switch!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        auto queryStr =
            QString("UPDATE Hosts SET switch_id = '0' WHERE switch_id = '%1';")
            .arg(id);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" updating error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }

        queryStr =
            QString("DELETE FROM Switches WHERE id = '%1'")
            .arg(id);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Switches\" deleting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::selectHosts()
    {
        const auto queryStr =
            QString("SELECT Hosts.id, Hosts.name, Hosts.domain_name, Hosts.ip, Hosts.interface, Hosts.switch_port, Switches.name "
                    "FROM Hosts INNER JOIN Switches ON Switches.id = Hosts.switch_id;");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
        else
        {
            emit hostsUpdatedSignal(query);
        }
    }
    void VMTDDeviceManager::createHost(int id, const QString &name,
                                       const QString &domainName,
                                       const QString &ip,
                                       const QString &interface,
                                       const QString &switchport,
                                       int switchId)
    {
        if (id == 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of host!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        QString queryStr;

        if (id > 0)
        {
            queryStr =
                QString("UPDATE Hosts SET name = '%1', domain_name = '%2', ip = '%3', interface = '%4', switch_port = '%5', switch_id = '%6'"
                        "WHERE id = '%7';")
                .arg(name)
                .arg(domainName)
                .arg(ip)
                .arg(interface)
                .arg(switchport)
                .arg(switchId)
                .arg(id);
        }
        else if (id < 0)
        {
            queryStr =
                QString("INSERT INTO Hosts (name, domain_name, ip, interface, switch_port, switch_id) "
                        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6');")
                .arg(name)
                .arg(domainName)
                .arg(ip)
                .arg(interface)
                .arg(switchport)
                .arg(switchId);
        }

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" inserting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }
    void VMTDDeviceManager::removeHost(int id)
    {
        if (id <= 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of host!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        const auto queryStr =
            QString("DELETE FROM Hosts WHERE id = '%1'")
            .arg(id);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" deleting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }

    const QList<VMTDParticipant *> &VMTDDeviceManager::participants() const
    {
        return m_participants;
    }
    void VMTDDeviceManager::buildParticipants()
    {
        qDeleteAll(m_participants);
        m_participants.clear();

        const auto queryStr =
            QString("SELECT Hosts.id, Hosts.name, Hosts.domain_name, Hosts.ip, Hosts.interface, Hosts.switch_port, Switches.name, Switches.url "
                    "FROM Hosts INNER JOIN Switches ON Switches.id = Hosts.switch_id WHERE Hosts.id > '0';");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }

        while (query.next())
        {
            const auto hostIp = query.value("Hosts.ip").toString();
            const auto switchUrl = query.value("Switches.url").toString();

            if (m_settings->shouldCheckOnline() &&
                (!m_urlOnline.contains(switchUrl) || !m_ipOnline.contains(hostIp)))
                continue;

            const auto hostName = query.value("Hosts.name").toString();
            const auto hostDomainName = query.value("Hosts.domain_name").toString();
            const auto hostInterface = query.value("Hosts.interface").toString();
            const auto switchPort = query.value("Hosts.switch_port").toString();
            const auto switchName = query.value("Switches.name").toString();

            auto participant = new VMTDParticipant(this);
            participant->setDefaultVlanId(m_settings->defaultVlanId());
            participant->setHostName(hostName);
            participant->setHostDomainName(hostDomainName);
            participant->setHostIp(hostIp);
            participant->setHostInterface(hostInterface);
            participant->setSwitchName(switchName);
            participant->setSwitchUrl(switchUrl);
            participant->setSwitchPort(switchPort);
            m_participants.append(participant);
        }
    }

    const QList<QString> &VMTDDeviceManager::hostIps() const
    {
        return m_hostIps;
    }
    void VMTDDeviceManager::buildHostIps()
    {
        m_hostIps.clear();

        const auto queryStr =
            QString("SELECT ip FROM Hosts WHERE id > '0';");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Hosts\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }

        while (query.next())
        {
            const auto hostIp = query.value("ip").toString();
            m_hostIps.append(hostIp);
        }
    }

    const QList<QUrl> &VMTDDeviceManager::switchUrls() const
    {
        return m_switchUrls;
    }
    void VMTDDeviceManager::buildSwitchUrls()
    {
        m_switchUrls.clear();

        const auto queryStr =
            QString("SELECT url, username, password FROM Switches WHERE id > '0';");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Switches\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }

        while (query.next())
        {
            const auto switchUrl = query.value("url").toString();
            const auto switchUsername = query.value("username").toString();
            const auto switchPassword = query.value("password").toString();

            QUrl url;
            url.setUrl(switchUrl);
            url.setUserName(switchUsername);
            url.setPassword(switchPassword);

            m_switchUrls.append(url);
        }
    }

    void VMTDDeviceManager::selectBannedScanners()
    {
        const auto queryStr =
            QString("SELECT Banned_scanners.id, Hosts.name, Banned_scanners.remaining "
                    "FROM Banned_scanners INNER JOIN Hosts ON Hosts.id = Banned_scanners.host_id;");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Banned_scanners\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
        else
        {
            emit bannedScannersUpdatedSignal(query);
        }
    }
    void VMTDDeviceManager::createBannedScanner(int id, int hostId, int remaining)
    {
        QString queryStr;

        if (id >= 0)
        {
            queryStr =
                QString("UPDATE Banned_scanners SET host_id = '%1', remaining = '%2' WHERE id = '%3';")
                .arg(hostId)
                .arg(remaining)
                .arg(id);
        }
        else if (id < 0)
        {
            queryStr =
                QString("INSERT INTO Banned_scanners (host_id, remaining) VALUES ('%1', '%2');")
                .arg(hostId)
                .arg(remaining);
        }

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Banned_scanners\" inserting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }
    void VMTDDeviceManager::removeBannedScanner(int id)
    {
        if (id < 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of banned scanner!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        const auto queryStr =
            QString("DELETE FROM Banned_scanners WHERE id = '%1'")
            .arg(id);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Banned_scanners\" deleting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::selectAllowedScanners()
    {
        const auto queryStr =
            QString("SELECT Allowed_scanners.id, Hosts.name "
                    "FROM Allowed_scanners INNER JOIN Hosts ON Hosts.id = Allowed_scanners.host_id;");

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Allowed_scanners\" selecting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
        else
        {
            emit allowedScannersUpdatedSignal(query);
        }
    }
    void VMTDDeviceManager::createAllowedScanner(int id, int hostId)
    {
        QString queryStr;

        if (id >= 0)
        {
            queryStr =
                QString("UPDATE Allowed_scanners SET host_id = '%1' WHERE id = '%2';")
                .arg(hostId)
                .arg(id);
        }
        else if (id < 0)
        {
            queryStr =
                QString("INSERT INTO Allowed_scanners (host_id) VALUES ('%1');")
                .arg(hostId);
        }

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Allowed_scanners\" inserting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }
    void VMTDDeviceManager::removeAllowedScanner(int id)
    {
        if (id < 0)
        {
            m_settings->debugOut(QString("%1 | Bad id value of allowed scanner!")
                                 .arg(VN_S(VMTDDeviceManager)));
            return;
        }

        const auto queryStr =
            QString("DELETE FROM Allowed_scanners WHERE id = '%1'")
            .arg(id);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"Allowed_scanners\" deleting error: %2")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::apply()
    {
        emit restartSignal();
    }

    void VMTDDeviceManager::updateUrlOnlineSlot(const QString &url, bool isOnline)
    {
        m_urlOnline[url] = isOnline;
    }
    void VMTDDeviceManager::updateIpOnlineSlot(const QString &ip, bool isOnline)
    {
        m_ipOnline[ip] = isOnline;
    }

    void VMTDDeviceManager::printDbConnections()
    {
        QString dbConnections = VN_S(VMTDDeviceManager) + " | All DB connections: ";

        for (auto connection : QSqlDatabase::connectionNames())
            dbConnections += connection + "; ";

        m_settings->debugOut(dbConnections);
    }

    void VMTDDeviceManager::createTables()
    {
        if (!isDbOpened())
            return;

        createTableSwitches();
        createTableHosts();
        createTableBannedScanners();
        createTableAllowedScanners();
    }

    void VMTDDeviceManager::createTableSwitches()
    {
        const auto tableName = QString("Switches");

        auto queryStr =
            QString("CREATE TABLE IF NOT EXISTS %1 ("
                    "id INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE DEFAULT (1), "
                    "name STRING NOT NULL, "
                    "url STRING NOT NULL, "
                    "username STRING NOT NULL, "
                    "password STRING NOT NULL);")
            .arg(tableName);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" creating error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }

        queryStr =
            QString("INSERT OR IGNORE INTO %1 (id, name, url, username, password) "
                    "VALUES ('0', 'none', 'none', 'none', 'none');")
            .arg(tableName);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" inserting error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::createTableHosts()
    {
        const auto tableName = QString("Hosts");

        auto queryStr =
            QString("CREATE TABLE IF NOT EXISTS %1 ("
                    "id INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE DEFAULT (1), "
                    "name STRING NOT NULL, "
                    "domain_name STRING NOT NULL, "
                    "ip STRING NOT NULL, "
                    "interface STRING NOT NULL, "
                    "switch_port STRING NOT NULL,"
                    "switch_id INTEGER REFERENCES Switches (Id) NOT NULL);")
            .arg(tableName);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" creating error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }

        queryStr =
            QString("INSERT OR IGNORE INTO %1 (id, name, domain_name, ip, interface, switch_port, switch_id) "
                    "VALUES ('0', 'none', 'none', 'none', 'none', 'none', '0');")
            .arg(tableName);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" inserting error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::createTableBannedScanners()
    {
        const auto tableName = QString("Banned_scanners");

        auto queryStr =
            QString("CREATE TABLE IF NOT EXISTS %1 ("
                    "id INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE DEFAULT (1), "
                    "host_id INTEGER REFERENCES Hosts (Id) NOT NULL, "
                    "remaining INTEGER NOT NULL);")
            .arg(tableName);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" creating error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }
    }

    void VMTDDeviceManager::createTableAllowedScanners()
    {
        const auto tableName = QString("Allowed_scanners");

        auto queryStr =
            QString("CREATE TABLE IF NOT EXISTS %1 ("
                    "id INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE DEFAULT (1), "
                    "host_id INTEGER REFERENCES Hosts (Id) NOT NULL);")
            .arg(tableName);

        QSqlQuery query(m_db);

        if (!query.exec(queryStr))
        {
            m_settings->debugOut(QString("%1 | Table \"%2\" creating error: %3")
                                 .arg(VN_S(VMTDDeviceManager))
                                 .arg(tableName)
                                 .arg(query.lastError().text()));
        }
    }
}
