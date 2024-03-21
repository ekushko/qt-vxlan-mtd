#include    "VMTDHostProtocolHandlerForm.h"
#include "ui_VMTDHostProtocolHandlerForm.h"

#include <QJsonDocument>

namespace VMTDLib
{
    VMTDHostProtocolHandlerForm::VMTDHostProtocolHandlerForm(QWidget *parent,
                                                             VMTDHostProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDHostProtocolHandlerForm),
        m_handler(handler)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Protocol %1:%2").
                       arg(m_handler->ip())
                       .arg(m_handler->port()));

        m_handlerForm = new VMTDProtocolHandlerForm(ui->gbProtocolHandler, m_handler);

        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDHostProtocolHandler::checkConnectionSlot);
        connect(this, &VMTDHostProtocolHandlerForm::appendRequestListSignal,
                m_handler, &VMTDHostProtocolHandler::appendRequestListSlot);

        if (m_handler->side() == VMTDProtocolHandler::EnSide::CLIENT)
            ui->gbRequest->hide();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDHostProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);
    }

    VMTDHostProtocolHandlerForm::~VMTDHostProtocolHandlerForm()
    {
        delete ui;
    }

    void VMTDHostProtocolHandlerForm::uiTimerTickSlot()
    {
        m_handlerForm->updateView();
    }

    void VMTDLib::VMTDHostProtocolHandlerForm::on_pbSend_clicked()
    {
        const auto text = ui->pteParams->toPlainText();
        const auto jsonDoc = QJsonDocument::fromJson(text.toUtf8());

        RequestList requests;
        requests.append(qMakePair(ui->leMethod->text(), jsonDoc.object()));
        emit appendRequestListSignal(requests);
    }
}
