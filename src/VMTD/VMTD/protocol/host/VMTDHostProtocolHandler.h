#pragma once

#include "../VMTDProtocolHandler.h"
#include "../../VMTDAlias.h"

#include <QWebSocket>
#include <QJsonDocument>

namespace VMTDLib
{
    class VMTDHostProtocolHandlerForm;

    class VMTDHostProtocolHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDHostProtocolHandler(QObject *parent,
                                VMTDSettings *settings,
                                EnSide side,
                                const QString &hostIp, int hostPort);

        // КОНСТАНТЫ

        enum class EnError
        {
            // -32000..32099 резерв
            PARSE_ERROR      = -32700,
            INVALID_REQUEST  = -32600,
            METHOD_NOT_FOUND = -32601,
            INVALID_PARAMS   = -32602,
            INTERNAL_ERROR   = -32603
        };
        Q_ENUM(EnError)
        static const QString            &enErrorToS(const EnError &error);
        static const QMap<int, QString> &enErrorToL();

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString hostIp() const;

        int hostPort() const;

        int queueLength() const override;

    signals:

        void sendMessageSignal(const QString &ip, int port, const QString &data);

        void handleMethodSignal(const QString &method, const QJsonObject &params, bool &result);

        void updateIpOnlineSignal(const QString &ip, bool isOnline);

    public slots:

        void appendRequestListSlot(const RequestList &requests);

        void receiveMessageSlot(const QString &ip, int port, const QString &data);

        void clearQueueSlot() override;

    private:

        bool           isRequest(const QJsonObject &request) const;
        QJsonObject buildRequest(const QString &method, const QJsonObject &params);

        bool           isResponse(const QJsonObject &response) const;
        QJsonObject buildResponse(const QJsonValue &id, const QJsonValue &result);
        QJsonObject buildError(const QJsonValue &id, int code, const QString &message);

        void handleServer(const QJsonObject &response);
        void handleClient(const QJsonObject &request, QJsonObject &response);

        QPointer<VMTDHostProtocolHandlerForm> m_form;

        QString m_ip;
        int m_port;

        int m_commandCounter = 0;

        QQueue<QJsonDocument> m_requests;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}
