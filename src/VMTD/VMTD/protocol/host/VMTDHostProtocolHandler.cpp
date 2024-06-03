#include "VMTDHostProtocolHandler.h"
#include "VMTDHostProtocolHandlerForm.h"

#include "../../VMTDVarname.h"
#include "../../VMTDMethod.h"
#include "../../VMTDTemplate.h"

#include <QJsonDocument>

namespace VMTDLib
{
    VMTDHostProtocolHandler::VMTDHostProtocolHandler(QObject *parent,
                                                     VMTDSettings *settings,
                                                     EnSide side,
                                                     const QString &ip, int port)
        : VMTDProtocolHandler(parent, settings, EnType::HOST, side)
        , m_ip(ip)
        , m_port(port)
    {

    }

    const QString &VMTDHostProtocolHandler::enErrorToS(const EnError &error)
    {
        switch (error)
        {
        // *INDENT-OFF*
        case EnError::PARSE_ERROR      : RETURN_S("Parse error");
        case EnError::INVALID_REQUEST  : RETURN_S("Invalid request");
        case EnError::METHOD_NOT_FOUND : RETURN_S("Method not found");
        case EnError::INVALID_PARAMS   : RETURN_S("Invalid params");
        case EnError::INTERNAL_ERROR   : RETURN_S("Internal error");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDHostProtocolHandler::enErrorToL()
    {
        RETURN_MAP(EnError, enErrorToS);
    }

    void VMTDHostProtocolHandler::checkConnection()
    {
        QList<QPair<QString, QJsonObject>> requests;

        requests.append(qMakePair(MTH_CHECK_CONNECTION, QJsonObject()));

        appendRequestListSlot(requests);
    }

    QString VMTDHostProtocolHandler::hostIp() const
    {
        return m_ip;
    }

    int VMTDHostProtocolHandler::hostPort() const
    {
        return m_port;
    }

    int VMTDHostProtocolHandler::queueLength() const
    {
        return m_requests.length();
    }

    void VMTDHostProtocolHandler::appendRequestListSlot(const RequestList &requests)
    {
        if (m_side != EnSide::SERVER)
            return;

        QJsonDocument jsonDoc;

        if (requests.size() > 1)
        {
            QJsonArray jsonArr;

            for (const auto &p : requests)
                jsonArr.append(buildRequest(p.first, p.second));

            jsonDoc.setArray(jsonArr);
            m_requests.append(jsonDoc);
        }
        else if (requests.size() == 1)
        {
            const auto &p = requests.at(0);

            jsonDoc.setObject(buildRequest(p.first, p.second));
            m_requests.append(jsonDoc);
        }
    }

    void VMTDHostProtocolHandler::receiveMessageSlot(const QString &ip, int port, const QString &data)
    {
        if (ip != hostIp() && port != hostPort())
        {
            const auto debugText = QString("Received from %1:%2 but expected %3:%4")
                                   .arg(ip).arg(port)
                                   .arg(hostIp()).arg(hostPort());
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                 .arg(VN_S(VMTDHostProtocolHandler))
                                 .arg(hostIp()).arg(hostPort())
                                 .arg(debugText));
        }

        if (data.isEmpty())
        {
            const auto debugText = "Empty data";
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                 .arg(VN_S(VMTDHostProtocolHandler))
                                 .arg(hostIp()).arg(hostPort())
                                 .arg(debugText));
            return;
        }

        QJsonParseError parseError;
        QJsonDocument inputDoc, outputDoc;

        inputDoc = QJsonDocument::fromJson(data.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError)
        {
            if (m_side == EnSide::CLIENT)
            {
                const auto debugText = QString("Parsing request error (%1): %2\n")
                                       .arg(parseError.error)
                                       .arg(parseError.errorString());
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));

                outputDoc.setObject(buildError(QJsonValue(),
                                               (int)EnError::PARSE_ERROR,
                                               enErrorToS(EnError::PARSE_ERROR)));
            }
            else if (m_side == EnSide::SERVER)
            {
                const auto debugText = QString("Parsing response error: %1\n").arg(parseError.errorString());
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));
            }
        }
        else
        {
            if (m_side == EnSide::CLIENT)
            {
                const auto debugText = QString("Request received:\n")
                                       + inputDoc.toJson(QJsonDocument::JsonFormat::Indented);
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));
            }
            else if (m_side == EnSide::SERVER)
            {
                const auto debugText = QString("Response received:\n")
                                       + inputDoc.toJson(QJsonDocument::JsonFormat::Indented);
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));
            }

            if (inputDoc.isObject())
            {
                if (m_side == EnSide::CLIENT)
                {
                    QJsonObject jsonObj;

                    handleClient(inputDoc.object(), jsonObj);

                    if (!jsonObj.isEmpty())
                        outputDoc.setObject(jsonObj);
                }
                else if (m_side == EnSide::SERVER)
                {
                    handleServer(inputDoc.object());
                }

            }
            else if (inputDoc.isArray())
            {
                QJsonArray jsonArr;

                for (int i = 0; i < inputDoc.array().size(); ++i)
                {
                    QJsonObject jsonObj;

                    if (m_side == EnSide::CLIENT)
                    {
                        handleClient(inputDoc.array().at(i).toObject(), jsonObj);

                        if (!jsonObj.isEmpty())
                            jsonArr.append(jsonObj);
                    }
                    else if (m_side == EnSide::SERVER)
                    {
                        handleServer(inputDoc.array().at(i).toObject());
                    }
                }

                if (m_side == EnSide::CLIENT && !jsonArr.isEmpty())
                    outputDoc.setArray(jsonArr);
            }
        }

        if (m_side == EnSide::CLIENT && !outputDoc.isEmpty())
        {
            emit sendMessageSignal(ip, port, outputDoc.toJson(QJsonDocument::JsonFormat::Indented));

            const auto debugText = QString("Response sent:\n")
                                   + outputDoc.toJson(QJsonDocument::JsonFormat::Indented);
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                 .arg(VN_S(VMTDHostProtocolHandler))
                                 .arg(hostIp()).arg(hostPort())
                                 .arg(debugText));
        }
    }

    void VMTDHostProtocolHandler::clearQueueSlot()
    {
        m_requests.clear();
    }

    bool VMTDHostProtocolHandler::isRequest(const QJsonObject &request) const
    {
        bool isValid = true;

        isValid &= request.size() >= 2;
        isValid &= request.contains("jsonrpc") && request["jsonrpc"].toString() == "2.0";
        isValid &= request.contains("method") && request["method"].isString();

        return isValid;
    }
    QJsonObject VMTDHostProtocolHandler::buildRequest(const QString &method, const QJsonObject &params)
    {
        QJsonObject jsonObj;

        jsonObj["jsonrpc"] = "2.0";
        jsonObj["method"] = method;
        jsonObj["params"] = params;
        jsonObj["id"] = QString("%1").arg(m_commandCounter++);

        return jsonObj;
    }

    bool VMTDHostProtocolHandler::isResponse(const QJsonObject &response) const
    {
        bool isValid = true;

        isValid &= response.size() >= 2;
        isValid &= response.contains("jsonrpc") && response["jsonrpc"].toString() == "2.0";

        if (response.contains("error") && response["error"].isObject())
        {
            const auto errorObj = response["error"].toObject();

            isValid &= errorObj.contains("code");
            isValid &= !errorObj["code"].isNull();

            isValid &= errorObj.contains("message");
            isValid &= errorObj["message"].isString();
        }
        else if (response.contains("result"))
        {
            isValid = true;
        }
        else
        {
            isValid = false;
        }

        return isValid;
    }
    QJsonObject VMTDHostProtocolHandler::buildResponse(const QJsonValue &id,
                                                       const QJsonValue &result)
    {
        QJsonObject jsonObj;

        jsonObj["jsonrpc"] = "2.0";
        jsonObj["result"] = result;
        jsonObj["id"] = id;

        return jsonObj;
    }
    QJsonObject VMTDHostProtocolHandler::buildError(const QJsonValue &id,
                                                    int code, const QString &message)
    {
        QJsonObject jsonObj, errorObj;

        errorObj["code"] = code;
        errorObj["message"] = message;

        jsonObj["jsonrpc"] = "2.0";
        jsonObj["error"] = errorObj;
        jsonObj["id"] = id;

        return jsonObj;
    }

    void VMTDHostProtocolHandler::handleServer(const QJsonObject &response)
    {
        emit updateIpOnlineSignal(hostIp(), true);

        m_ticketTimeoutTimer.stop();

        if (!isResponse(response))
        {
            const auto debugText = QString("Handling response error: Invalid response");
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                 .arg(VN_S(VMTDHostProtocolHandler))
                                 .arg(hostIp()).arg(hostPort())
                                 .arg(debugText));
        }
        else
        {
            if (response.contains("error"))
            {
                const auto errorObj = response["error"].toObject();

                const auto debugText = QString("Response error: %1\n").arg(errorObj["message"].toString());
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));
            }
            else if (response.contains("result"))
            {
                if (response["result"].isBool())
                {
                    bool isOk = response["result"].toBool();

                    const auto debugText = QString("Request handled %1!").arg(isOk ? "successful" : "with error");
                    emit showDebugSignal(QTime::currentTime(), debugText);
                    m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                         .arg(VN_S(VMTDHostProtocolHandler))
                                         .arg(hostIp()).arg(hostPort())
                                         .arg(debugText));
                }
                else
                {
                    emit handleResultSignal(response["result"]);

                    const auto debugText = QString("Request handled but result should be handled too!");
                    emit showDebugSignal(QTime::currentTime(), debugText);
                    m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                         .arg(VN_S(VMTDHostProtocolHandler))
                                         .arg(hostIp()).arg(hostPort())
                                         .arg(debugText));
                }
            }
            else
            {
                const auto debugText = QString("Request handled but result is unknown!");
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));
            }
        }

        m_queueState = EnQueueState::READY_TO_SEND;
    }
    void VMTDHostProtocolHandler::handleClient(const QJsonObject &request, QJsonObject &response)
    {
        if (!isRequest(request))
        {
            const auto error = EnError::INVALID_REQUEST;

            const auto debugText = QString("Handling request error: %1\n").arg(enErrorToS(error));
            emit showDebugSignal(QTime::currentTime(), debugText);
            m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                 .arg(VN_S(VMTDHostProtocolHandler))
                                 .arg(hostIp()).arg(hostPort())
                                 .arg(debugText));

            response = buildError(request["id"], (int)error, enErrorToS(error));
        }
        else
        {
            const auto method = request["method"].toString();

            if (!VMTDMethod::methods().values().contains(method))
            {
                const auto error = EnError::METHOD_NOT_FOUND;

                const auto debugText = QString("Handling request error: %1\n").arg(enErrorToS(error));
                emit showDebugSignal(QTime::currentTime(), debugText);
                m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                     .arg(VN_S(VMTDHostProtocolHandler))
                                     .arg(hostIp()).arg(hostPort())
                                     .arg(debugText));

                response = buildError(request["id"], (int)error, enErrorToS(error));
            }
            else
            {
                bool isValid = true;

                const auto methodName = VMTDMethod::methods().key(method);
                const auto methodParams = VMTDMethod::methodParams(methodName);

                auto params = request["params"].toObject();

                for (auto methodParam : methodParams)
                {
                    auto waitedParam = VMTDMethod::params().value(methodParam);

                    if (!params.contains(waitedParam) || !VMTDMethod::checkParam(methodParam, params[waitedParam]))
                    {
                        const auto error = EnError::INVALID_PARAMS;

                        const auto debugText = QString("Handling request error: %1\n").arg(enErrorToS(error));
                        emit showDebugSignal(QTime::currentTime(), debugText);
                        m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                             .arg(VN_S(VMTDHostProtocolHandler))
                                             .arg(hostIp()).arg(hostPort())
                                             .arg(debugText));

                        response = buildError(request["id"], (int)error, enErrorToS(error));

                        isValid = false;

                        break;
                    }
                }

                if (isValid)
                {
                    QJsonValue result = false;

                    const auto debugText = QString("Method is valid: %1\n").arg(method);
                    emit showDebugSignal(QTime::currentTime(), debugText);
                    m_settings->debugOut(QString("%1 | %2:%3 | %4")
                                         .arg(VN_S(VMTDHostProtocolHandler))
                                         .arg(hostIp()).arg(hostPort())
                                         .arg(debugText));

                    emit handleMethodSignal(method, params, result);

                    response = buildResponse(request["id"], result);
                }
            }
        }
    }

    void VMTDHostProtocolHandler::checkQueueTimerSlot()
    {
        if (m_requests.isEmpty()
            || m_queueState != EnQueueState::READY_TO_SEND)
            return;

        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        QJsonDocument outputDoc = m_requests.dequeue();

        const auto debugText = QString("Message sent:\n")
                               + outputDoc.toJson(QJsonDocument::JsonFormat::Indented);
        emit showDebugSignal(QTime::currentTime(), debugText);
        m_settings->debugOut(QString("%1 | %2:%3 | %4")
                             .arg(VN_S(VMTDHostProtocolHandler))
                             .arg(hostIp()).arg(hostPort())
                             .arg(debugText));

        emit sendMessageSignal(hostIp(), hostPort(), outputDoc.toJson(QJsonDocument::JsonFormat::Indented));

        m_ticketTimeoutTimer.start();
    }

    void VMTDHostProtocolHandler::ticketTimeoutSlot()
    {
        m_queueState = EnQueueState::READY_TO_SEND;

        emit updateIpOnlineSignal(hostIp(), false);

        const auto debugText = QString("Response not received");
        emit showDebugSignal(QTime::currentTime(), debugText);
        m_settings->debugOut(QString("%1 | %2:%3 | %4")
                             .arg(VN_S(VMTDHostProtocolHandler))
                             .arg(hostIp()).arg(hostPort())
                             .arg(debugText));
    }
}
