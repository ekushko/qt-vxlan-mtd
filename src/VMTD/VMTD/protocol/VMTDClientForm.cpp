#include    "VMTDClientForm.h"
#include "ui_VMTDClientForm.h"

namespace VMTDLib
{
    VMTDClientForm::VMTDClientForm(QWidget *parent, VMTDClient *client) :
        QWidget(parent),
        ui(new Ui::VMTDClientForm),
        m_client(client)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(m_client, &VMTDClient::handlerCreatedSignal,
                this, &VMTDClientForm::handlerCreatedSlot);
        connect(m_client, &VMTDClient::handlerRemovedSignal,
                this, &VMTDClientForm::handlerRemovedSlot);
    }

    VMTDClientForm::~VMTDClientForm()
    {
        delete ui;
    }

    void VMTDClientForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr
            || handler->type() != VMTDProtocolHandler::EnType::HOST)
            return;

        auto hostHandler = dynamic_cast<VMTDHostProtocolHandler *>(handler);
        m_hostForm = new VMTDHostProtocolHandlerForm(ui->wHandler, hostHandler);
    }
    void VMTDClientForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr
            || handler->type() != VMTDProtocolHandler::EnType::HOST)
            return;

        if (m_hostForm != nullptr)
            delete m_hostForm;
    }
}
