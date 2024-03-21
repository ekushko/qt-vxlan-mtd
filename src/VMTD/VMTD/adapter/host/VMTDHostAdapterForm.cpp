#include    "VMTDHostAdapterForm.h"
#include "ui_VMTDHostAdapterForm.h"

#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDHostAdapterForm::VMTDHostAdapterForm(QWidget *parent, QWebSocket *socket) :
        QWidget(parent),
        ui(new Ui::VMTDHostAdapterForm),
        m_socket(socket)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        m_adapterForm = new VMTDAdapterForm(ui->wAdapter);
    }

    VMTDHostAdapterForm::~VMTDHostAdapterForm()
    {
        delete ui;
    }

    void VMTDHostAdapterForm::showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text)
    {
        if (m_socket == socket)
        {
            m_adapterForm->appendText(QString("\n[%1] %2\n")
                                      .arg(time.toString("hh:mm:ss:zzz"))
                                      .arg(text));
        }
    }
}
