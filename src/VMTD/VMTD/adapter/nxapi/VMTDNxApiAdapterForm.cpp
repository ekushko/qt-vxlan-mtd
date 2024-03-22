#include    "VMTDNxApiAdapterForm.h"
#include "ui_VMTDNxApiAdapterForm.h"

namespace VMTDLib
{
    VMTDNxApiAdapterForm::VMTDNxApiAdapterForm(QWidget *parent, VMTDNxApiAdapter *adapter) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiAdapterForm),
        m_adapter(adapter)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setWindowTitle(QString("NX-API Адаптер (%1)").arg(m_adapter->url().toString()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(m_adapter, &VMTDNxApiAdapter::showDebugSignal,
                this, &VMTDNxApiAdapterForm::showDebugSlot,
                Qt::QueuedConnection);

        initializeView();
        updateView();
    }

    VMTDNxApiAdapterForm::~VMTDNxApiAdapterForm()
    {
        delete ui;
    }

    void VMTDNxApiAdapterForm::updateView()
    {
        // do nothing
    }

    void VMTDNxApiAdapterForm::showDebugSlot(const QTime &time, const QString &text)
    {
        m_adapterForm->appendText(QString("\n[%1] %2\n")
                                  .arg(time.toString("hh:mm:ss:zzz"))
                                  .arg(text));
    }

    void VMTDNxApiAdapterForm::initializeView()
    {
        m_adapterForm = new VMTDAdapterForm(ui->wAdapter);
    }
}
