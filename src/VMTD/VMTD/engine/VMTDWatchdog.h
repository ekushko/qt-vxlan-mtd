#pragma once

#include "../VMTDSettings.h"

#include <QFileSystemWatcher>

namespace VMTDLib
{
    class VMTDWatchdog : public QObject
    {
        Q_OBJECT

    public:

        VMTDWatchdog(QObject *parent, VMTDSettings *settings);

    private:

        VMTDSettings *m_settings;

        QFileSystemWatcher m_watcher;

    private slots:

        void fileChangedSlot(const QString &path);
    };
}
