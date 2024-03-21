#pragma once

#include "VMTDHostServer.h"
#include "VMTDHostAdapterForm.h"

namespace Ui
{
    class VMTDHostServerForm;
}

namespace VMTDLib
{
    class VMTDHostServerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDHostServerForm(QWidget *parent, VMTDHostServer *server);
        ~VMTDHostServerForm();

    public slots:

        void showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text);

    private:

        void initializeView();
        void updateView();

        Ui::VMTDHostServerForm *ui;

        VMTDHostServer *m_server;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

        QMap<QWebSocket *, VMTDHostAdapterForm *> m_socketToForm;

    private slots:

        void uiTimerTickSlot();

        void appendClientSocketSlot(QWebSocket *socket);
        void removeClientSocketSlot(QWebSocket *socket);

        void pbShowDetailedStateClicked();
        void on_pbShowDetailedState_clicked();
    };
}
