#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "VMTD/VMTDSettings.h"
#include "VMTD/device/VMTDDeviceManager.h"
#include "VMTD/engine/VMTDEngine.h"
using namespace VMTDLib;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto settings = new VMTDSettings(this, "_vmtd");

    auto deviceManager = new VMTDDeviceManager(this, settings);
    deviceManager->showForm();

    auto engine = new VMTDEngine(this, settings, deviceManager);
    engine->showForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

