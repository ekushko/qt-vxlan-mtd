#include "VMTDWatchdog.h"

#include "../VMTDVarname.h"

#include <QFile>
#include <QDebug>
#include <QRegularExpression>

namespace VMTDLib
{
    static const QString PATTERN =
        "(?<timestamp>\\d\\d\\/\\d\\d-\\d\\d:\\d\\d:\\d\\d.\\d+) "
        "(?<src_ip>\\d+.\\d+.\\d+.\\d+):(?<src_port>\\d+) -> "
        "(?<dst_ip>\\d+.\\d+.\\d+.\\d+):(?<dst_port>\\d+)";

    VMTDWatchdog::VMTDWatchdog(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_shouldWait = false;
        m_shouldCheck = false;

        if (!QFile::exists(m_settings->alertFilePath()))
        {
            m_settings->debugOut(QString("%1 | Alert file (%2) not found!")
                                 .arg(VN_S(VMTDWatchdog))
                                 .arg(m_settings->alertFilePath()));
        }
        else
        {
            QFile::resize(m_settings->alertFilePath(), 0);

            if (m_settings->shouldControlAlert())
            {
                m_watcher.addPath(m_settings->alertFilePath());

                connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &VMTDWatchdog::fileChangedSlot);
            }
        }

        m_delayTimer.setSingleShot(true);
        connect(&m_delayTimer, &QTimer::timeout, this, &VMTDWatchdog::delayTimerTickSlot);
    }

    const QList<QString> &VMTDWatchdog::scanners() const
    {
        return m_scanners;
    }

    void VMTDWatchdog::fileChangedSlot(const QString &path)
    {
        if (m_shouldWait)
        {
            m_shouldCheck = true;
            return;
        }

        m_settings->debugOut(QString("%1 | Alert detected %2!")
                             .arg(VN_S(VMTDWatchdog))
                             .arg(path));

        QFile f(m_settings->alertFilePath());

        if (!f.open(QIODevice::ReadOnly))
        {
            m_settings->debugOut(QString("%1 | File %2 not opened!")
                                 .arg(VN_S(VMTDWatchdog))
                                 .arg(m_settings->alertFilePath()));
            return;
        }

        const auto alertContent = QString::fromUtf8(f.readAll());
        f.close();

        const QRegularExpression re(PATTERN);
        auto matchIt = re.globalMatch(alertContent);

        m_scanners.clear();

        while (matchIt.hasNext())
        {
            const auto match = matchIt.next();

            if (match.hasMatch())
            {
                const auto timeStamp = match.captured("timestamp");
                const auto srcIp = match.captured("src_ip");
                const auto srcPort = match.captured("src_port");
                const auto dstIp = match.captured("dst_ip");
                const auto dstPort = match.captured("dst_port");

                m_settings->debugOut(QString("%1 | Scan [%2] %3:%4 -> %5:%6")
                                     .arg(VN_S(VMTDWatchdog))
                                     .arg(timeStamp)
                                     .arg(srcIp).arg(srcPort)
                                     .arg(dstIp).arg(dstPort));

                m_scanners.append(srcIp);
            }
        }

        if (!m_scanners.isEmpty())
            emit scanDetectedSignal(m_scanners);

        if (QFile::exists(m_settings->alertFilePath()))
        {
            QFile::resize(m_settings->alertFilePath(), 0);

            m_watcher.addPath(m_settings->alertFilePath());
        }

        m_shouldCheck = false;
        m_shouldWait = true;
        m_delayTimer.start(m_settings->alertDelayInterval());
    }

    void VMTDWatchdog::delayTimerTickSlot()
    {
        m_shouldWait = false;

        if (m_shouldCheck)
            fileChangedSlot(m_settings->alertFilePath());
    }
}
