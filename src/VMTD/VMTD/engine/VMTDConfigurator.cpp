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

    VMTDConfigurator::VMTDConfigurator(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        connect(this, &VMTDConfigurator::applyNetplanSignal,
                this, &VMTDConfigurator::applyNetplanSlot);

        m_netplan1 = load(netplan1FilePath());
        m_netplan2 = load(netplan2FilePath());
        //m_hosts = load(hostsFilePath());
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
        return QString();
    }


    void VMTDConfigurator::handleMethodSlot(const QString &method,
                                            const QJsonObject &params,
                                            bool &result)
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
                             .arg(result ? "successfully" : "with error")
                             .arg(method));
    }

    bool VMTDConfigurator::handleSetupInterface1(const QJsonObject &params)
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

        return true;
    }
    bool VMTDConfigurator::handleSetupInterface2(const QJsonObject &params)
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

        return true;
    }
    bool VMTDConfigurator::handleSetupHosts(const QJsonObject &params)
    {
        m_hosts.clear();

        const auto &jsonArr = params[PRM_HOSTS].toArray();

        if (!jsonArr.isEmpty())
        {
            for (auto jsonVal : params[PRM_HOSTS].toArray())
            {
                const auto jsonObj = jsonVal.toObject();

                m_hosts.append(QString("%1 %2\n")
                               .arg(params[PRM_IP].toString())
                               .arg(params[PRM_DOMAIN_NAME].toString()));
            }
        }

        return true;
    }

    bool VMTDConfigurator::handleClearInterface1(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan1.clear();

        return true;
    }
    bool VMTDConfigurator::handleClearInterface2(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_netplan2.clear();

        return true;
    }
    bool VMTDConfigurator::handleClearHosts(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_hosts.clear();

        return true;
    }

    bool VMTDConfigurator::handleApplyNetplan(const QJsonObject &params)
    {
        Q_UNUSED(params)

        emit applyNetplanSignal();

        return true;
    }

    bool VMTDConfigurator::handleCheckConnection(const QJsonObject &params)
    {
        Q_UNUSED(params)

        return true;
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


    void VMTDConfigurator::applyNetplanSlot()
    {
        save(netplan1FilePath(), m_netplan1);
        save(netplan2FilePath(), m_netplan2);
        //save(hostsFilePath(), m_hosts);

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
