#pragma once

#include "../VMTDSettings.h"

#include <QFileSystemWatcher>
#include <QTimer>

namespace VMTDLib
{
    class VMTDWatchdog : public QObject
    {
        Q_OBJECT

    public:

        VMTDWatchdog(QObject *parent, VMTDSettings *settings);

        const QList<QString> &scanners() const;

    signals:

        void scanDetectedSignal(const QList<QString> scanners);

    private:

        VMTDSettings *m_settings;

        QFileSystemWatcher m_watcher;

        QList<QString> m_scanners;

        QTimer m_delayTimer;
        bool m_shouldWait;
        bool m_shouldCheck;

    private slots:

        void fileChangedSlot(const QString &path);

        void delayTimerTickSlot();
    };
}
