#include    "VMTDHostClientForm.h"
#include "ui_VMTDHostClientForm.h"

namespace VMTDLib
{
    VMTDHostClientForm::VMTDHostClientForm(QWidget *parent, VMTDHostClient *client) :
        QWidget(parent),
        ui(new Ui::VMTDHostClientForm),
        m_client(client),
        m_settings(m_client->settings())
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);

        m_adapterForm = new VMTDHostAdapterForm(ui->wLeft, m_client->socket());
        m_adapterForm->layout()->setMargin(0);
        connect(m_client, &VMTDHostClient::showDebugSignal,
                m_adapterForm, &VMTDHostAdapterForm::showDebugSlot);
        ui->wLeft->layout()->addWidget(m_adapterForm);

        initializeView();

        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDHostClientForm::uiTimerTickSlot);
        m_uiTimer.start(500);
    }

    VMTDHostClientForm::~VMTDHostClientForm()
    {
        delete ui;
    }

    void VMTDHostClientForm::initializeView()
    {
        connect(ui->pbDisconnectSocket, &QPushButton::clicked,
                m_client, &VMTDHostClient::disconnectSocketSlot);
        connect(ui->pbConnectSocket, &QPushButton::clicked,
                m_client, &VMTDHostClient::connectSocketSlot);
    }

    void VMTDHostClientForm::updateView()
    {
        if (m_client == nullptr)
        {
            ui->lbSocketState->setText("State: None");
            ui->lbOpen->setText("Opened: No");
            ui->lbLocalAdress->setText("Local IP: None");
            ui->lbPeerAdress->setText("Remote IP: None");

            setWindowTitle("VMTD Host-client: н/д:н/д");

            return;
        }

        const QString title = QString("VMTD Host-client: %1:%2")
                              .arg(m_settings->serverIp())
                              .arg(m_settings->serverPort());
        setWindowTitle(title);

        const auto socketState = m_client->socket()->state();

        ui->lbSocketState->setText("State: " + QVariant::fromValue(socketState).toString());

        if (socketState == QAbstractSocket::ConnectedState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(true);
            ui->lbOpen->setText("Opened: Yes");
        }
        else if (socketState == QAbstractSocket::ConnectingState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Opened: No");
        }
        else
        {
            ui->pbConnectSocket->setEnabled(true);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Opened: No");
        }

        QString ipAdress;

        if (!m_client->socket()->localAddress().isNull())
        {
            ipAdress = QString("Local IP: %1:%2")
                       .arg(m_client->socket()->localAddress().toString())
                       .arg(m_client->socket()->localPort());
            ui->
            lbLocalAdress->setText(ipAdress);
        }
        else
        {
            ui->lbLocalAdress->setText("Local IP: None");
        }

        if (!m_client->socket()->peerAddress().isNull())
        {
            ipAdress = QString("Remote IP: %1:%2")
                       .arg(m_client->socket()->peerAddress().toString())
                       .arg(m_client->socket()->peerPort());
            ui->lbPeerAdress->setText(ipAdress);
        }
        else
        {
            ui->lbPeerAdress->setText("Remote IP: None");
        }
    }

    void VMTDHostClientForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDHostClientForm::on_pbShowDetailedState_clicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? ">" : "<");
    }
}


