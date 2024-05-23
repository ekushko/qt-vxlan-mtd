#pragma once

#include "VMTDEngine.h"

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui
{
    class VMTDEngineForm;
}

namespace VMTDLib
{
    class VMTDEngineForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDEngineForm(QWidget *parent, VMTDEngine *engine);
        ~VMTDEngineForm();

    private:

        void updateView();

        void fillInterface(VMTDInterface *interface, QTreeWidgetItem *item);

        Ui::VMTDEngineForm *ui;

        VMTDEngine *m_engine;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void on_pbGenerate_clicked();

        void on_pbRun_clicked();
    };
}
