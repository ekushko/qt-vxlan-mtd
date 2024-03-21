#include    "VMTDAdapterForm.h"
#include "ui_VMTDAdapterForm.h"

namespace VMTDLib
{
    VMTDAdapterForm::VMTDAdapterForm(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::VMTDAdapterForm)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);
    }

    VMTDAdapterForm::~VMTDAdapterForm()
    {
        delete ui;
    }

    void VMTDAdapterForm::appendText(const QString &text)
    {
        if (ui->chbShouldUpdate->isChecked())
            ui->pteFlow->appendPlainText(text);
    }

    void VMTDAdapterForm::on_pbClear_clicked()
    {
        ui->pteFlow->clear();
    }
}
