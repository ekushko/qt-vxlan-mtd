#include "VMTDConfigurator.h"
#include "VMTDConfiguratorForm.h"

#include "../VMTDVarname.h"
#include "../VMTDMethod.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include <QDir>
#include <QFile>
#include <QProcess>

#include <QRegularExpression>

namespace VMTDLib
{
    static const QString NETPLAN_PATH_TEMPLATE = QString("%1%2%3%4%5")
                                                 .arg(QDir::separator())
                                                 .arg("etc")
                                                 .arg(QDir::separator())
                                                 .arg("netplan")
                                                 .arg(QDir::separator());

    static const QString HOSTS_BEG_TAG = QString("# *HOSTS BEGIN*");
    static const QString HOSTS_END_TAG = QString("# *HOSTS END*");

    VMTDConfigurator::VMTDConfigurator(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_watchdog = new VMTDWatchdog(this, m_settings);

        connect(this, &VMTDConfigurator::applyNetplanSignal,
                this, &VMTDConfigurator::applyNetplanSlot);

        m_netplan1 = load(netplan1FilePath());
        m_netplan2 = load(netplan2FilePath());
        m_hosts = loadHosts(hostsFilePath());
    }

    VMTDConfigurator::~VMTDConfigurator()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QWidget *VMTDConfigurator::form() const
    {
        return m_form;
    }
    void VMTDConfigurator::showForm(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDConfiguratorForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDConfigurator::settings() const
    {
        return m_settings;
    }

    const QString &VMTDConfigurator::netplan1() const
    {
        return m_netplan1;
    }
    void VMTDConfigurator::setNetplan1(const QString &netplan1)
    {
        QRegularExpression re("vlan.[0-9]+");
        const auto rm = re.match(m_netplan1);
        m_vlanInterface1 = rm.captured(0);

        m_netplan1 = netplan1;
    }
    QString VMTDConfigurator::netplan1FilePath() const
    {
        return NETPLAN_PATH_TEMPLATE
               + QString("%1.%2")
               .arg(m_settings->netplan1FileName())
               .arg("yaml");
    }

    const QString &VMTDConfigurator::netplan2() const
    {
        return m_netplan2;
    }
    void VMTDConfigurator::setNetplan2(const QString &netplan2)
    {
        QRegularExpression re("vlan.[0-9]+");
        const auto rm = re.match(m_netplan2);
        m_vlanInterface2 = rm.captured(0);

        m_netplan2 = netplan2;
    }
    QString VMTDConfigurator::netplan2FilePath() const
    {
        return NETPLAN_PATH_TEMPLATE
               + QString("%1.%2")
               .arg(m_settings->netplan2FileName())
               .arg("yaml");
    }

    const QString &VMTDConfigurator::hosts() const
    {
        return m_hosts;
    }
    void VMTDConfigurator::setHosts(const QString &hosts)
    {
        m_hosts = hosts;
    }
    QString VMTDConfigurator::hostsFilePath() const
    {
        return QString("%1%2%3%4")
               .arg(QDir::separator())
               .arg("etc")
               .arg(QDir::separator())
               .arg("hosts");
    }


    void VMTDConfigurator::handleMethodSlot(const QString &method,
                                            const QJsonObject &params,
                                            QJsonValue &result)
    {
        m_settings->debugOut(QString("%1 | Start handling method: %2")
                             .arg(VN_S(VMTDConfigurator))
                             .arg(method));

        if (method == MTH_SETUP_INTERFACE_1)
        {
            result = handleSetupInterface1(params);
        }
        else if (method == MTH_SETUP_INTERFACE_2)
        {
            result = handleSetupInterface2(params);
        }
        else if (method == MTH_SETUP_HOSTS)
        {
            result = handleSetupHosts(params);
        }
        else if (method == MTH_CLEAR_INTERFACE_1)
        {
            result = handleClearInterface1(params);
        }
        else if (method == MTH_CLEAR_INTERFACE_2)
        {
            result = handleClearInterface2(params);
        }
        else if (method == MTH_CLEAR_HOSTS)
        {
            result = handleClearHosts(params);
        }
        else if (method == MTH_GET_SCANNERS)
        {
            result = handleGetScanners(params);
        }
        else if (method == MTH_APPLY_NETPLAN)
        {
            result = handleApplyNetplan(params);
        }
        else if (method == MTH_CHECK_CONNECTION)
        {
            result = handleCheckConnection(params);
        }
        else
        {
            result = false;
        }

        m_settings->debugOut(QString("%1 | Method handled %2: %3")
                             .arg(VN_S(VMTDConfigurator))
                             .arg(result.toBool() ? "successfully" : "with error")
                             .arg(method));
    }

    QJsonValue VMTDConfigurator::handleSetupInterface1(const QJsonObject &params)
    {
        auto netplan = VLAN_HEADER_TEMPLATE +
                       QString(VLAN_TEMPLATE)
                       .arg(params[PRM_VLAN_ID].toInt())
                       .arg(params[PRM_INTERFACE].toString())
                       .arg(params[PRM_IP].toString())
                       .arg(params[PRM_MASK].toInt())
                       .arg(params[PRM_MAC].toString());

        if (params.contains(PRM_ROUTES)
            && params[PRM_ROUTES].isArray())
        {
            const auto routesArr = params[PRM_ROUTES].toArray();

            if (!routesArr.isEmpty())
            {
                netplan += ROUTE_HEADER_TEMPLATE;

                for (const auto routeVal : routesArr)
                {
                    const auto &routeObj = routeVal.toObject();

                    netplan += QString(ROUTE_TEMPLATE)
                               .arg(routeObj[PRM_NETWORK].toString())
                               .arg(routeObj[PRM_MASK].toInt())
                               .arg(routeObj[PRM_GATEWAY].toString())
                               .arg(routeObj[PRM_METRIC].toInt());
                }
            }
        }

        setNetplan1(netplan);

        return QJsonValue(true);
    }
    QJsonValue VMTDConfigurator::handleSetupInterface2(const QJsonObject &params)
    {
        auto netplan = VLAN_HEADER_TEMPLATE +
                       QString(VLAN_TEMPLATE)
                       .arg(params[PRM_VLAN_ID].toInt())
                       .arg(params[PRM_INTERFACE].toString())
                       .arg(params[PRM_IP].toString())
                       .arg(params[PRM_MASK].toInt())
                       .arg(params[PRM_MAC].toString());

        if (params.contains(PRM_ROUTES)
            && params[PRM_ROUTES].isArray())
        {
            const auto routesArr = params[PRM_ROUTES].toArray();

            if (!routesArr.isEmpty())
            {
                netplan += ROUTE_HEADER_TEMPLATE;

                for (const auto routeVal : routesArr)
                {
                    const auto &routeObj = routeVal.toObject();

                    netplan += QString(ROUTE_TEMPLATE)
                               .arg(routeObj[PRM_NETWORK].toString())
                               .arg(routeObj[PRM_MASK].toInt())
                               .arg(routeObj[PRM_GATEWAY].toString())
                               .arg(routeObj[PRM_METRIC].toInt());
                }
            }
        }

        setNetplan2(netplan);

        return QJsonValue(true);
    }
    QJsonValue VMTDConfigurator::handleSetupHosts(const QJsonObject &params)
    {
        m_hosts.clear();

        const auto &jsonArr = params[PRM_HOSTS].toArray();

        if (!jsonArr.isEmpty())
        {
            for (const auto jsonVal : jsonArr)
            {
                const auto &itemObj = jsonVal.toObject();

                m_hosts.append(QString("%1 %2\n")
                               .arg(itemObj[PRM_IP].toString())
                               .arg(itemObj[PRM_DOMAIN_NAME].toString()));
            }
        }

        return QJsonValue(true);
    }

    QJsonValue VMTDConfigurator::handleClearInterface1(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan1.clear();

        return QJsonValue(true);
    }
    QJsonValue VMTDConfigurator::handleClearInterface2(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan2.clear();

        return QJsonValue(true);
    }
    QJsonValue VMTDConfigurator::handleClearHosts(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_hosts.clear();

        return QJsonValue(true);
    }

    QJsonValue VMTDConfigurator::handleGetScanners(const QJsonObject &params)
    {
        Q_UNUSED(params)

        QJsonObject jsonObj;
        QJsonArray jsonArr;

        for (auto scannerIp : m_watchdog->scanners())
        {
            QJsonObject itemObj;
            itemObj[PRM_IP] = scannerIp;
            jsonArr.append(itemObj);
        }

        jsonObj[PRM_SCANNERS] = jsonArr;

        m_watchdog->clearScanners();

        return QJsonValue(jsonObj);
    }

    QJsonValue VMTDConfigurator::handleApplyNetplan(const QJsonObject &params)
    {
        Q_UNUSED(params)

        emit applyNetplanSignal();

        return QJsonValue(true);
    }

    QJsonValue VMTDConfigurator::handleCheckConnection(const QJsonObject &params)
    {
        Q_UNUSED(params)

        return QJsonValue(true);
    }

    void VMTDConfigurator::save(const QString &filePath, const QString &data)
    {
        QFile f(filePath);

        if (!f.open(QIODevice::WriteOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(filePath));
            return;
        }

        f.write(data.toUtf8());
        f.close();
    }
    QString VMTDConfigurator::load(const QString &filePath)
    {
        QFile f(filePath);

        if (!f.open(QIODevice::ReadOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(filePath));
            return QString();
        }

        return QString::fromUtf8(f.readAll());
    }

    void VMTDConfigurator::saveHosts(const QString &filePath, const QString &data)
    {
        QFile f(filePath);

        if (!f.open(QIODevice::ReadOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(filePath));
            return;
        }

        auto text = QString::fromUtf8(f.readAll());

        f.close();


        const auto begPos = text.indexOf(HOSTS_BEG_TAG);

        if (begPos > 0)
        {
            const auto endPos = text.lastIndexOf(HOSTS_END_TAG);

            text.remove(begPos, endPos + HOSTS_END_TAG.size() - begPos);
            text.insert(begPos, QString("%1\n%2\n%3")
                        .arg(HOSTS_BEG_TAG)
                        .arg(data)
                        .arg(HOSTS_END_TAG));
        }
        else
        {
            text.append(QString("\n%1\n%2\n%3\n")
                        .arg(HOSTS_BEG_TAG)
                        .arg(data)
                        .arg(HOSTS_END_TAG));
        }


        if (!f.open(QIODevice::WriteOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(filePath));
            return;
        }

        f.write(text.toUtf8());
        f.close();
    }
    QString VMTDConfigurator::loadHosts(const QString &filePath)
    {
        QFile f(filePath);

        if (!f.open(QIODevice::ReadOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(filePath));
            return QString();
        }

        auto text = QString::fromUtf8(f.readAll());

        auto begPos = text.indexOf(HOSTS_BEG_TAG);

        if (begPos < 0)
            return QString();

        auto endPos = text.lastIndexOf(HOSTS_END_TAG);

        if (endPos < 0)
            endPos = text.size();

        begPos += HOSTS_BEG_TAG.size();

        return text.mid(begPos, endPos - begPos);
    }

    void VMTDConfigurator::applyNetplanSlot()
    {
        save(netplan1FilePath(), m_netplan1);
        save(netplan2FilePath(), m_netplan2);
        saveHosts(hostsFilePath(), m_hosts);

        QString commands;

        if (!m_vlanInterface1.isEmpty())
        {
            commands += QString("sudo ip link delete %1")
                        .arg(m_vlanInterface1);

            m_settings->debugOut(QString("%1 | Interface %2 is down!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(m_vlanInterface1));
        }

        if (!m_vlanInterface2.isEmpty())
        {
            if (!commands.isEmpty())
                commands += "; ";

            commands += QString("sudo ip link delete %1")
                        .arg(m_vlanInterface2);

            m_settings->debugOut(QString("%1 | Interface %2 is down!")
                                 .arg(VN_S(VMTDConfigurator))
                                 .arg(m_vlanInterface2));
        }

        // TODO 1.3 (KEA) Шлюзовые подсети
        // Исправить на удаление только тех маршрутов, которые были добавлены
        // удалить отсюда
        if (!commands.isEmpty())
            commands += "; ";

        commands += "sudo ip route flush table main";
        // удалить досюда


        {
            if (!commands.isEmpty())
                commands += "; ";

            commands += "sudo netplan apply";
        }

        const auto result =
            QProcess::execute("/bin/bash",
                              QStringList()
                              << "-c"
                              << commands);

        if (!result)
        {
            m_settings->debugOut(QString("%1 | Netplan not applied!")
                                 .arg(VN_S(VMTDConfigurator)));
        }
        else
        {
            m_settings->debugOut(QString("%1 | Netplan applied!")
                                 .arg(VN_S(VMTDConfigurator)));
        }
    }
}
