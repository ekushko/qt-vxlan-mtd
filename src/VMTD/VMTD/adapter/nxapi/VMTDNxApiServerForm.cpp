#include    "VMTDNxApiServerForm.h"
#include "ui_VMTDNxApiServerForm.h"

namespace VMTDLib
{
    VMTDNxApiServerForm::VMTDNxApiServerForm(QWidget *parent, VMTDNxApiServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiServerForm),
        m_server(server)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("NX-API Adapters");

        connect(m_server, &VMTDNxApiServer::adapterCreatedSignal,
                this, &VMTDNxApiServerForm::adapterCreatedSlot);
        connect(m_server, &VMTDNxApiServer::adapterRemovedSignal,
                this, &VMTDNxApiServerForm::adapterRemovedSlot);

        for (auto adapter : m_server->adapters())
            adapterCreatedSlot(adapter);

        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::stopListenSlot);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout,
                this, &VMTDNxApiServerForm::uiTimerTickSlot);
        m_uiTimer.start(500);

        uiTimerTickSlot();
    }

    VMTDNxApiServerForm::~VMTDNxApiServerForm()
    {
        delete ui;
    }

    void VMTDNxApiServerForm::uiTimerTickSlot()
    {
        ui->pbStart->setEnabled(!m_server->isListening());
        ui->pbStop->setEnabled(m_server->isListening());
        ui->lbListening->setText(QString("Listening: ") + (m_server->isListening() ? "Yes" : "No"));
        ui->lbAdapters->setText(QString("Adapters: %1").arg(m_server->adapters().size()));

        for (auto form : m_adapterForms.values())
            form->updateView();
    }

    void VMTDNxApiServerForm::adapterCreatedSlot(VMTDNxApiAdapter *adapter)
    {
        auto form = new VMTDNxApiAdapterForm(ui->twAdapters, adapter);
        ui->twAdapters->addTab(form, adapter->url().toString(QUrl::RemoveUserInfo));
        m_adapterForms[adapter] = form;
    }
    void VMTDNxApiServerForm::adapterRemovedSlot(VMTDNxApiAdapter *adapter)
    {
        delete m_adapterForms[adapter];
        m_adapterForms.remove(adapter);

        for (int i = 0; ui->twAdapters->tabBar()->count(); ++i)
        {
            if (ui->twAdapters->tabBar()->tabText(i) == adapter->url().toString(QUrl::RemoveUserInfo))
            {
                ui->twAdapters->removeTab(i);
                break;
            }
        }
    }

    void VMTDNxApiServerForm::on_pbShowDetailedState_clicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? ">" : "<");
    }
}
