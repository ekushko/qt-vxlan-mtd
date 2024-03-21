#include "VMTDNxApiAdapter.h"

#include "../../VMTDVarname.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNxApiAdapter::VMTDNxApiAdapter(QObject *parent,
                                       VMTDSettings *settings,
                                       QNetworkAccessManager *netManager,
                                       QUrl url)
        : QObject(parent)
        , m_settings(settings)
        , m_netManager(netManager)
        , m_url(url)
    {
        connect(m_netManager, &QNetworkAccessManager::finished,
                this, &VMTDNxApiAdapter::replyFinishedSlot);
    }

    QUrl VMTDNxApiAdapter::url() const
    {
        return m_url;
    }

    void VMTDNxApiAdapter::checkConnectionSlot()
    {
        QNetworkRequest request(m_url);
        buildRequest(&request);
        showRequest(&request);

        m_netManager->get(request);
    }

    void VMTDNxApiAdapter::executeCommandListSlot(const CommandList &commands)
    {
        QJsonArray jsonArray;

        for (const auto &command : commands)
        {
            QJsonObject jsonObj, paramsObj;

            jsonObj["jsonrpc"] = "2.0";
            jsonObj["method"] = "cli_ascii";

            paramsObj["cmd"] = command;
            paramsObj["version"] = 1;

            jsonObj["params"] = paramsObj;
            jsonObj["id"] = m_commandCounter++;

            jsonArray.append(jsonObj);
        }

        QJsonDocument jsonDoc;
        jsonDoc.setArray(jsonArray);

        QNetworkRequest request(m_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json-rpc");
        buildRequest(&request);
        showRequest(&request, &jsonDoc);

        m_netManager->post(request, jsonDoc.toJson());
    }

    void VMTDNxApiAdapter::replyFinishedSlot(QNetworkReply *reply)
    {
        if (!reply->isFinished())
            return;

        QString debugString = "Response received:\n";

        if (reply->error() == QNetworkReply::NoError)
            debugString.append(reply->readAll());
        else
            debugString.append(reply->errorString());

        emit commandListExecutedSignal(reply->error() != QNetworkReply::ConnectionRefusedError,
                                       reply->error() != QNetworkReply::NoError);

        emit showDebugSignal(QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + QString("(%1) | %2")
                             .arg(m_url.toString(QUrl::RemoveUserInfo))
                             .arg(debugString));

        reply->deleteLater();
    }

    void VMTDNxApiAdapter::buildRequest(QNetworkRequest *request)
    {
        auto conf = request->sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request->setSslConfiguration(conf);
    }
    void VMTDNxApiAdapter::showRequest(QNetworkRequest *request, QJsonDocument *jsonDoc)
    {
        QString debugString = "Request sent:\n" + request->url().toString();

        const QList<QByteArray> &rawHeaderList(request->rawHeaderList());

        foreach (QByteArray rawHeader, rawHeaderList)
            debugString.append("\n" + request->rawHeader(rawHeader));

        if (jsonDoc != nullptr)
            debugString.append("\n" + jsonDoc->toJson(QJsonDocument::JsonFormat::Indented));

        emit showDebugSignal(QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + QString("(%1) | %2")
                             .arg(m_url.toString(QUrl::RemoveUserInfo))
                             .arg(debugString));
    }
}
