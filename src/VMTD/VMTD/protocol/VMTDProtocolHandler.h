#pragma once

#include "../VMTDSettings.h"

#include <QQueue>
#include <QTimer>

namespace VMTDLib
{
    class VMTDProtocolHandler : public QObject
    {
        Q_OBJECT

    public:

        enum class EnType;

        enum class EnSide;

        VMTDProtocolHandler(QObject *parent, VMTDSettings *settings, EnType type, EnSide side);

        // КОНСТАНТЫ

        enum class EnType
        {
            NX_API,
            HOST
        };
        Q_ENUM(EnType)

        enum class EnSide
        {
            SERVER,
            CLIENT
        };
        Q_ENUM(EnSide)

        enum class EnQueueState
        {
            READY_TO_SEND,
            WAIT_FOR_TICKET
        };
        Q_ENUM(EnQueueState)

        // ЛОГИКА

        virtual void checkConnection() = 0;

        // МЕТАДАННЫЕ

        VMTDSettings *settings() const;

        EnQueueState queueState() const;

        // ДАННЫЕ

        EnType type() const;

        EnSide side() const;

        virtual int queueLength() const = 0;

    signals:

        void showDebugSignal(const QTime &time, const QString &text);

    public slots:

        virtual void showFormSlot() = 0;

        virtual void checkConnectionSlot();

        virtual void clearQueueSlot() = 0;

    protected:

        VMTDSettings *m_settings;

        EnType m_type;

        EnSide m_side;

        EnQueueState m_queueState;

        QTimer m_checkQueueTimer;

        QTimer m_ticketTimeoutTimer;

    protected slots:

        virtual void checkQueueTimerSlot() = 0;

        virtual void ticketTimeoutSlot() = 0;
    };
}
