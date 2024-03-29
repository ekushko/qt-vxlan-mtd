#include "VMTDNxApiProtocolHandler.h"
#include "VMTDNxApiProtocolHandlerForm.h"

#include "../../VMTDVarname.h"

namespace VMTDLib
{
    VMTDNxApiProtocolHandler::VMTDNxApiProtocolHandler(QObject *parent,
                                                       VMTDSettings *settings,
                                                       const QString &url)
        : VMTDProtocolHandler(parent, settings, EnType::NX_API, EnSide::SERVER)
        , m_url(url)
    {

    }

    void VMTDNxApiProtocolHandler::checkConnection()
    {
        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        emit checkConnectionSignal();

        const auto debugText = QString("Check connection!");
        emit showDebugSignal(QTime::currentTime(), debugText);
        m_settings->debugOut(QString("%1 | %2 | %3")
                             .arg(VN_S(VMTDNxApiProtocolHandler))
                             .arg(url())
                             .arg(debugText));

        m_ticketTimeoutTimer.start();
    }

    QString VMTDNxApiProtocolHandler::url() const
    {
        return m_url;
    }

    int VMTDNxApiProtocolHandler::queueLength() const
    {
        return m_queue.length();
    }

    void VMTDNxApiProtocolHandler::appendCommandListSlot(const CommandList &commands)
    {
        m_queue.enqueue(commands);
    }

    void VMTDNxApiProtocolHandler::commandListExecutedSlot(bool isOnline, bool hasError)
    {
        if (m_queueState == EnQueueState::WAIT_FOR_TICKET)
        {
            if (m_ticketTimeoutTimer.isActive())
                m_ticketTimeoutTimer.stop();

            emit updateUrlOnlineSignal(url(), isOnline);

            if (isOnline)
            {
                const auto debugText = QString("Command %1").arg((!hasError ? "executed!" : "failed!"));
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2 | %3")
                                     .arg(VN_S(VMTDNxApiProtocolHandler))
                                     .arg(url())
                                     .arg(debugText));
            }
            else
            {
                const auto debugText = QString("Response not received!");
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2 | %3")
                                     .arg(VN_S(VMTDNxApiProtocolHandler))
                                     .arg(url())
                                     .arg(debugText));
            }

            m_queueState = EnQueueState::READY_TO_SEND;
        }
        else
        {
            const auto debugText = QString("Unexpected response!");
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2 | %3")
                                 .arg(VN_S(VMTDNxApiProtocolHandler))
                                 .arg(url())
                                 .arg(debugText));
        }
    }

    void VMTDNxApiProtocolHandler::clearQueueSlot()
    {
        m_queueState = EnQueueState::READY_TO_SEND;
        m_queue.clear();
    }

    void VMTDNxApiProtocolHandler::checkQueueTimerSlot()
    {
        if (m_queue.isEmpty()
            || m_queueState != EnQueueState::READY_TO_SEND)
            return;

        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        const auto command = m_queue.dequeue();
        emit executeCommandListSignal(command);

        const auto debugText = QString("Command sent:\n%1").arg(command.join('\n'));
        emit showDebugSignal(QTime::currentTime(), debugText);
        m_settings->debugOut(QString("%1 | %2 | %3")
                             .arg(VN_S(VMTDNxApiProtocolHandler))
                             .arg(url())
                             .arg(debugText));

        m_ticketTimeoutTimer.start();
    }

    void VMTDNxApiProtocolHandler::ticketTimeoutSlot()
    {
        const auto debugText = QString("Command not executed!");
        emit showDebugSignal(QTime::currentTime(), debugText);
        m_settings->debugOut(QString("%1 | %2 | %3")
                             .arg(VN_S(VMTDNxApiProtocolHandler))
                             .arg(url())
                             .arg(debugText));

        m_queueState = EnQueueState::READY_TO_SEND;

        emit updateUrlOnlineSignal(url(), false);
    }
}
