#pragma once

#include <QWidget>
#include <QTimer>

#include "VMTDNxApiAdapter.h"
#include "../VMTDAdapterForm.h"

namespace Ui
{
    class VMTDNxApiAdapterForm;
}

namespace VMTDLib
{
    class VMTDNxApiAdapterForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiAdapterForm(QWidget *parent, VMTDNxApiAdapter *adapter);
        ~VMTDNxApiAdapterForm();

        void updateView();

    public slots:

        void showDebugSlot(const QTime &time, const QString &text);

    private:

        void initializeView();

        Ui::VMTDNxApiAdapterForm *ui;

        VMTDNxApiAdapter *m_adapter;
        VMTDSettings *m_settings;

        VMTDAdapterForm *m_adapterForm;
    };
}
