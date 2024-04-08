#include "MainWindow.h"
#include "Tray.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Tray t;

    if (a.arguments().contains("-tray"))
    {
        QObject::connect(&t, &Tray::showMainWindowSignal,
                         &w, &MainWindow::show);
        QObject::connect(&t, &Tray::showSettingsSignal,
                         &w, &MainWindow::showSettingsSlot);
        a.setQuitOnLastWindowClosed(false);

        t.show();
    }
    else
    {
        w.show();
    }

    return a.exec();
}
