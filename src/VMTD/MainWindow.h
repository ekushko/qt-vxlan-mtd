#pragma once

#include <QMainWindow>

#include "VMTD/VMTDController.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QPointer<VMTDLib::VMTDController> m_controller;

private slots:

    void on_actionDevice_triggered();
    void on_actionSettings_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();

    void on_actionNxApiServer_triggered();
    void on_actionHostServer_triggered();
    void on_actionServer_triggered();
    void on_actionEngine_triggered();
    void on_actionHostClient_triggered();
    void on_actionClient_triggered();
    void on_actionConfigurator_triggered();
};
