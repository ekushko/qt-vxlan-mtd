#include "Tray.h"

#include <QApplication>
#include <QAction>
#include <QMenu>

Tray::Tray(QWidget *parent)
    : QWidget(parent)
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QPixmap(":/favicon-tray.png"));

    m_trayIconMenu = new QMenu(this);

    auto showMainFormAction = new QAction("Main", m_trayIconMenu);
    connect(showMainFormAction, &QAction::triggered, this, &Tray::showMainFormSlot);
    m_trayIconMenu->addAction(showMainFormAction);

    auto settingsFormAction = new QAction("Settings", m_trayIconMenu);
    connect(settingsFormAction, &QAction::triggered, this, &Tray::showSettingsFormSlot);
    m_trayIconMenu->addAction(settingsFormAction);

    auto quitAction = new QAction("Exit", m_trayIconMenu);
    connect(quitAction, &QAction::triggered, this, &Tray::exitSlot);
    m_trayIconMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(m_trayIconMenu);
}

void Tray::show()
{
    m_trayIcon->show();
}
void Tray::hide()
{
    m_trayIcon->hide();
}

void Tray::showMainFormSlot()
{
    emit showMainWindowSignal();
}

void Tray::showSettingsFormSlot()
{
    emit showSettingsSignal();
}

void Tray::exitSlot()
{
    qApp->exit();
}
