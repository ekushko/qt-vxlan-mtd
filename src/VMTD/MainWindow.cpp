#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace VMTDLib;

template<typename T>
void showObjectForm(QStackedWidget *w, T object)
{
    object->showForm(w);

    if (w->indexOf(object->form()) < 0)
        w->addWidget(object->form());

    w->setCurrentWidget(object->form());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_controller = new VMTDController(this, "_vmtd");
    m_controller->startController();

    connect(m_controller->settings(), &VMTDSettings::restartSignal,
            this, &MainWindow::restartSlot);

    restartSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsSlot()
{
    m_controller->settings()->showForm();
}

void MainWindow::restartSlot()
{
    const auto isServer = (m_controller->settings()->nodeType() == VMTDNodeType::SERVER);
    const auto isRunning = m_controller->isRunning();

    ui->actionDevice->setEnabled(isServer);
    ui->actionStart->setEnabled(!isRunning);
    ui->actionStop->setEnabled(isRunning);

    ui->menuServer->setEnabled(isServer);
    ui->actionNxApiServer->setEnabled(isRunning);
    ui->actionHostServer->setEnabled(isRunning);
    ui->actionServer->setEnabled(isRunning);
    ui->actionEngine->setEnabled(isServer);

    ui->actionHostClient->setEnabled(isRunning);
    ui->actionClient->setEnabled(isRunning);
    ui->actionConfigurator->setEnabled(isRunning);
}

void MainWindow::on_actionDevice_triggered()
{
    auto manager = m_controller->manager();
    showObjectForm(ui->stackedWidget, manager);
}

void MainWindow::on_actionSettings_triggered()
{
    m_controller->settings()->showForm();
}

void MainWindow::on_actionStart_triggered()
{
    m_controller->startController();

    restartSlot();
}
void MainWindow::on_actionStop_triggered()
{
    m_controller->stopController();

    restartSlot();
}


void MainWindow::on_actionNxApiServer_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto nxApiServer = m_controller->nxApiServer();
    showObjectForm(ui->stackedWidget, nxApiServer);
}

void MainWindow::on_actionHostServer_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto hostServer = m_controller->hostServer();
    showObjectForm(ui->stackedWidget, hostServer);
}

void MainWindow::on_actionServer_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto server = m_controller->server();
    showObjectForm(ui->stackedWidget, server);
}

void MainWindow::on_actionEngine_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto engine = m_controller->engine();
    showObjectForm(ui->stackedWidget, engine);
}


void MainWindow::on_actionHostClient_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto hostClient = m_controller->hostClient();
    showObjectForm(ui->stackedWidget, hostClient);
}

void MainWindow::on_actionClient_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto client = m_controller->client();
    showObjectForm(ui->stackedWidget, client);
}

void MainWindow::on_actionConfigurator_triggered()
{
    if (!m_controller->isRunning())
        return;

    auto configurator = m_controller->configurator();
    showObjectForm(ui->stackedWidget, configurator);
}
