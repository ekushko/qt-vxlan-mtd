#include "VMTDProtocolHandler.h"

namespace VMTDLib
{
    VMTDProtocolHandler::VMTDProtocolHandler(QObject *parent,
                                             VMTDSettings *settings,
                                             EnType type, EnSide side)
        : QObject(parent)
        , m_settings(settings)
        , m_type(type)
        , m_side(side)
    {
        m_queueState = EnQueueState::READY_TO_SEND;

        m_checkQueueTimer.setParent(this);
        m_checkQueueTimer.setInterval(m_settings->checkQueueInterval());
        connect(&m_checkQueueTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::checkQueueTimerSlot);

        if (m_side == EnSide::SERVER)
            m_checkQueueTimer.start();

        m_ticketTimeoutTimer.setParent(this);
        m_ticketTimeoutTimer.setInterval(m_settings->ticketTimeoutInterval());
        connect(&m_ticketTimeoutTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::ticketTimeoutSlot);
        m_ticketTimeoutTimer.setSingleShot(true);
    }

    VMTDSettings *VMTDProtocolHandler::settings() const
    {
        return m_settings;
    }

    VMTDProtocolHandler::EnQueueState VMTDProtocolHandler::queueState() const
    {
        return m_queueState;
    }

    VMTDProtocolHandler::EnType VMTDProtocolHandler::type() const
    {
        return m_type;
    }

    VMTDProtocolHandler::EnSide VMTDProtocolHandler::side() const
    {
        return m_side;
    }

    void VMTDProtocolHandler::checkConnectionSlot()
    {
        checkConnection();
    }
}
