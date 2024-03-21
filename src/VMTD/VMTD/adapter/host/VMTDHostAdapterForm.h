#pragma once

#include "../VMTDAdapterForm.h"

#include <QWidget>
#include <QWebSocket>

namespace Ui
{
    class VMTDHostAdapterForm;
}

namespace VMTDLib
{
    class VMTDHostAdapterForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDHostAdapterForm(QWidget *parent, QWebSocket *socket);
        ~VMTDHostAdapterForm();

    public slots:

        void showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text);

    private:

        Ui::VMTDHostAdapterForm *ui;

        VMTDAdapterForm *m_adapterForm;

        QWebSocket *m_socket;
    };
}
