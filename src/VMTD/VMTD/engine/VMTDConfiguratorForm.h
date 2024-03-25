#pragma once

#include "VMTDConfigurator.h"

#include <QWidget>
#include <QTimer>

namespace Ui
{
    class VMTDConfiguratorForm;
}

namespace VMTDLib
{
    class VMTDConfiguratorForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDConfiguratorForm(QWidget *parent, VMTDConfigurator *configurator);
        ~VMTDConfiguratorForm();

    private:

        void updateView();

        Ui::VMTDConfiguratorForm *ui;

        VMTDConfigurator *m_configurator;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();
    };
}
