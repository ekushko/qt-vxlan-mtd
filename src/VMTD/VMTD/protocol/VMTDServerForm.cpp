#include    "VMTDServerForm.h"
#include "ui_VMTDServerForm.h"

namespace VMTDLib
{
    VMTDServerForm::VMTDServerForm(QWidget *parent, VMTDServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDServerForm),
        m_server(server)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(m_server, &VMTDServer::handlerCreatedSignal,
                this, &VMTDServerForm::handlerCreatedSlot);
        connect(m_server, &VMTDServer::handlerRemovedSignal,
                this, &VMTDServerForm::handlerRemovedSlot);

        for (auto handler : m_server->hostHandlers())
            handlerCreatedSlot(handler);

        for (auto handler : m_server->nxApiHandlers())
            handlerCreatedSlot(handler);
    }

    VMTDServerForm::~VMTDServerForm()
    {
        delete ui;
    }

    void VMTDServerForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
        {
            auto nxApiHandler = dynamic_cast<VMTDNxApiProtocolHandler *>(handler);
            auto form = new VMTDNxApiProtocolHandlerForm(ui->tbwNxApi, nxApiHandler);
            ui->tbwNxApi->addTab(form, nxApiHandler->url());
            m_nxApiForms[handler] = form;
        }
        else if (handler->type() == VMTDProtocolHandler::EnType::HOST)
        {
            auto hostHandler = dynamic_cast<VMTDHostProtocolHandler *>(handler);
            auto form = new VMTDHostProtocolHandlerForm(ui->tbwHost, hostHandler);
            ui->tbwHost->addTab(form, hostHandler->ip());
            m_hostForms[handler] = form;
        }
    }

    void VMTDServerForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
        {
            auto nxApiHandler = dynamic_cast<VMTDNxApiProtocolHandler *>(handler);

            for (int i = 0; ui->tbwNxApi->tabBar()->count(); ++i)
            {
                if (ui->tbwNxApi->tabBar()->tabText(i) == nxApiHandler->url())
                {
                    ui->tbwNxApi->removeTab(i);
                    break;
                }
            }

            delete m_nxApiForms[nxApiHandler];
            m_nxApiForms.remove(nxApiHandler);
        }
        else if (handler->type() == VMTDProtocolHandler::EnType::HOST)
        {
            auto hostHandler = dynamic_cast<VMTDHostProtocolHandler *>(handler);

            for (int i = 0; ui->tbwHost->tabBar()->count(); ++i)
            {
                if (ui->tbwHost->tabBar()->tabText(i) == hostHandler->ip())
                {
                    ui->tbwHost->removeTab(i);
                    break;
                }
            }

            delete m_hostForms[hostHandler];
            m_hostForms.remove(hostHandler);
        }
    }
}
