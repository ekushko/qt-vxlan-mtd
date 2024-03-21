#pragma once

#include "VMTDProtocolHandler.h"

#include <QWidget>
#include <QTimer>
#include <QTime>

namespace Ui
{
    class VMTDProtocolHandlerForm;
}

namespace VMTDLib
{
    class VMTDProtocolHandlerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDProtocolHandlerForm(QWidget *parent, VMTDProtocolHandler *handler);
        ~VMTDProtocolHandlerForm();

        void updateView();

    public slots:

        void showDebugSlot(const QTime &time, const QString &text);

    private:

        Ui::VMTDProtocolHandlerForm *ui;

        VMTDProtocolHandler *m_handler;
        VMTDSettings *m_settings;

    private slots:

        void pbClearFlowClicked();
        void on_pbClearFlow_clicked();
    };
}
