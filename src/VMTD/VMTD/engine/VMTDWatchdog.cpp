#include "VMTDWatchdog.h"

#include "../VMTDVarname.h"

#include <QFile>
#include <QDebug>

namespace VMTDLib
{
    VMTDWatchdog::VMTDWatchdog(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        if (!QFile::exists(m_settings->alertFilePath()))
        {
            m_settings->debugOut(QString("%1 | Alert file (%2) not found!")
                                 .arg(VN_S(VMTDWatchdog))
                                 .arg(m_settings->alertFilePath()));
        }
        else
        {
            m_watcher.addPath(m_settings->alertFilePath());
        }

        connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &VMTDWatchdog::fileChangedSlot);
    }

    void VMTDWatchdog::fileChangedSlot(const QString &path)
    {
        m_settings->debugOut(QString("%1 | Alert (%2)!")
                             .arg(VN_S(VMTDWatchdog))
                             .arg(path));

        if (QFile::exists(m_settings->alertFilePath()))
            m_watcher.addPath(m_settings->alertFilePath());
    }
}
