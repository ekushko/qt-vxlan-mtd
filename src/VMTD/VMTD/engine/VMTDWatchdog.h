#pragma once

#include "../VMTDSettings.h"

#include <QFileSystemWatcher>
#include <QTimer>
#include <QSet>

namespace VMTDLib
{
    class VMTDWatchdog : public QObject
    {
        Q_OBJECT

    public:

        VMTDWatchdog(QObject *parent, VMTDSettings *settings);

        const QSet<QString> &scanners() const;
        void            clearScanners();

    private:

        VMTDSettings *m_settings;

        QFileSystemWatcher m_watcher;

        QSet<QString> m_scanners;

        QTimer m_delayTimer;
        bool m_shouldWait;
        bool m_shouldCheck;

    private slots:

        void fileChangedSlot(const QString &path);

        void delayTimerTickSlot();
    };
}
