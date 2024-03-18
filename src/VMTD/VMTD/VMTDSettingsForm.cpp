#include    "VMTDSettingsForm.h"
#include "ui_VMTDSettingsForm.h"

namespace VMTDLib
{
    VMTDSettingsForm::VMTDSettingsForm(QWidget *parent, VMTDSettings *settings) :
        QWidget(parent),
        ui(new Ui::VMTDSettingsForm)
    {
        ui->setupUi(this);
    }

    VMTDSettingsForm::~VMTDSettingsForm()
    {
        delete ui;
    }
}
