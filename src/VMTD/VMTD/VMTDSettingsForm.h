#pragma once

#include "VMTDSettings.h"

#include <QWidget>

namespace Ui
{
    class VMTDSettingsForm;
}

namespace VMTDLib
{
    class VMTDSettingsForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDSettingsForm(QWidget *parent, VMTDSettings *settings);
        ~VMTDSettingsForm();

    private:

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

        Ui::VMTDSettingsForm *ui;

        VMTDSettings *m_settings;

    private slots:


        void on_pbSave_clicked();
        void on_pbLoad_clicked();
        void on_pbChange_clicked();
        void on_pbAccept_clicked();
        void on_pbCancel_clicked();
    };
}
