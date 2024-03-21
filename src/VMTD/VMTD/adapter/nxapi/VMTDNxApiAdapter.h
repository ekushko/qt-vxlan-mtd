#pragma once

#include "../../VMTDSettings.h"
#include "../../VMTDAlias.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>

namespace VMTDLib
{
    class VMTDNxApiAdapter : public QObject
    {
        Q_OBJECT

    public:

        VMTDNxApiAdapter(QObject *parent,
                         VMTDSettings *settings,
                         QNetworkAccessManager *netManager,
                         QUrl url);

        QUrl url() const;

    signals:

        void showDebugSignal(const QTime &time, const QString &text);

        void commandListExecutedSignal(bool isOnline, bool hasError);

    public slots:

        void checkConnectionSlot();

        void executeCommandListSlot(const CommandList &commands);

        void replyFinishedSlot(QNetworkReply *reply);

    private:

        void buildRequest(QNetworkRequest *request);
        void  showRequest(QNetworkRequest *request, QJsonDocument *jsonDoc = nullptr);

        VMTDSettings *m_settings;

        QNetworkAccessManager *m_netManager;

        QUrl m_url;

        int m_commandCounter = 1;
    };
}
