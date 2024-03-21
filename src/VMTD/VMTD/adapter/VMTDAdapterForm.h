#pragma once

#include <QWidget>

namespace Ui
{
    class VMTDAdapterForm;
}

namespace VMTDLib
{
    class VMTDAdapterForm : public QWidget
    {
        Q_OBJECT

    public:

        explicit VMTDAdapterForm(QWidget *parent = nullptr);
        ~VMTDAdapterForm();

        void appendText(const QString &text);

    private:

        Ui::VMTDAdapterForm *ui;

    private slots:

        void on_pbClear_clicked();
    };
}
