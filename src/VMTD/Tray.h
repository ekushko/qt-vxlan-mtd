#pragma once

#include <QWidget>
#include <QSystemTrayIcon>


class Tray : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Tray)

public:

    explicit Tray(QWidget *parent = nullptr);

    void show();
    void hide();

signals:

    void showMainWindowSignal();

    void showSettingsSignal();

private:

    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;

private slots:

    void showMainFormSlot();
    void showSettingsFormSlot();
    void exitSlot();
};
