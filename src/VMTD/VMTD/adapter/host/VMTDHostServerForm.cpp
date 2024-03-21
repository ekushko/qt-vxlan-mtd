#include    "VMTDHostServerForm.h"
#include "ui_VMTDHostServerForm.h"

namespace VMTDLib
{
    VMTDHostServerForm::VMTDHostServerForm(QWidget *parent, VMTDHostServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDHostServerForm),
        m_server(server),
        m_settings(server->settings())
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(m_server, &VMTDHostServer::showDebugSignal,
                this, &VMTDHostServerForm::showDebugSlot);

        for (auto socket : m_server->WsClientSockets)
            appendClientSocketSlot(socket);

        connect(m_server, &VMTDHostServer::clientConnectedSignal,
                this, &VMTDHostServerForm::appendClientSocketSlot);
        connect(m_server, &VMTDHostServer::clientDisconnectedSignal,
                this, &VMTDHostServerForm::removeClientSocketSlot);

        initializeView();

        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout,
                this, &VMTDHostServerForm::uiTimerTickSlot);
        m_uiTimer.start(500);
    }

    VMTDHostServerForm::~VMTDHostServerForm()
    {
        delete ui;
    }

    void VMTDHostServerForm::showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text)
    {
        if (socket == nullptr)
        {
            ui->pteFlow->appendPlainText(QString("\n[%1] %2\n")
                                         .arg(time.toString("hh:mm:ss:zzz"))
                                         .arg(text));
        }
    }

    void VMTDHostServerForm::initializeView()
    {
        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDHostServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDHostServer::stopListenSlot);
    }

    void VMTDHostServerForm::updateView()
    {
        if (m_server == nullptr)
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Running: None");
            ui->lbLocalAdress->setText("Listening: None");
            ui->lbClientSockets->setText("Connected clients: None");

            ui->lbServerName->setText("Server name: None");

            setWindowTitle("VMTD Host-сервер: None");

            return;
        }

        const auto title = QString("VMTD Host-сервер: %1")
                           .arg(m_settings->localPort());
        setWindowTitle(title);

        if (m_server->wsServer()->isListening())
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(true);
            ui->lbOpen->setText("Running: Yes");
            ui->lbLocalAdress->setText("Listening: "
                                       + QString::number(m_settings->serverPort()));
            ui->lbClientSockets->setText("Connected clients: "
                                         + QString::number(m_server->WsClientSockets.size()));
            ui->lbServerName->setText("Server name: "
                                      + m_server->wsServer()->serverName());
        }
        else
        {
            ui->pbStart->setEnabled(true);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Running: No");
            ui->lbLocalAdress->setText("Listening: No");
            ui->lbClientSockets->setText("Connected clients: None");
            ui->lbServerName->setText("Server name: None");
        }
    }

    void VMTDHostServerForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDHostServerForm::appendClientSocketSlot(QWebSocket *socket)
    {
        auto form = new VMTDHostAdapterForm(this, socket);

        connect(m_server, &VMTDHostServer::showDebugSignal,
                form, &VMTDHostAdapterForm::showDebugSlot);

        m_socketToForm.insert(socket, form);

        const QString tabTitle = QString("%1:%2")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort());

        ui->twSockets->addTab(form, tabTitle);
    }
    void VMTDHostServerForm::removeClientSocketSlot(QWebSocket *socket)
    {
        auto form = m_socketToForm.value(socket);

        if (form != nullptr)
        {
            m_socketToForm.remove(socket);
            delete form;
        }
    }

    void VMTDHostServerForm::on_pbShowDetailedState_clicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? ">" : "<");
    }
}
