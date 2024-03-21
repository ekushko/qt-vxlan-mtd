#include    "VMTDProtocolHandlerForm.h"
#include "ui_VMTDProtocolHandlerForm.h"

namespace VMTDLib
{
    VMTDProtocolHandlerForm::VMTDProtocolHandlerForm(QWidget *parent, VMTDProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDProtocolHandlerForm),
        m_handler(handler),
        m_settings(handler->settings())
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(m_handler, &VMTDProtocolHandler::showDebugSignal,
                this, &VMTDProtocolHandlerForm::showDebugSlot);

        connect(ui->pbClearQueue, &QPushButton::clicked,
                m_handler, &VMTDProtocolHandler::clearQueueSlot);

        if (m_handler->side() == VMTDProtocolHandler::EnSide::CLIENT)
        {
            ui->pbClearQueue->hide();
            ui->lbQueueLength->hide();
        }
    }

    VMTDProtocolHandlerForm::~VMTDProtocolHandlerForm()
    {
        delete ui;
    }

    void VMTDProtocolHandlerForm::updateView()
    {
        ui->lbQueueLength->setText(QString("Queue length: %1").arg(m_handler->queueLength()));
    }

    void VMTDProtocolHandlerForm::showDebugSlot(const QTime &time, const QString &text)
    {
        if (!ui->chbShouldUpdate->isChecked())
            return;

        ui->pteFlow->appendPlainText(QString("[%1] %2\n")
                                     .arg(time.toString("hh:mm:ss:zzz"))
                                     .arg(text));
    }

    void VMTDProtocolHandlerForm::on_pbClearFlow_clicked()
    {
        ui->pteFlow->clear();
    }
}
