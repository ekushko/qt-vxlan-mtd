#include    "VMTDDeviceTableForm.h"
#include "ui_VMTDDeviceTableForm.h"

namespace VMTDLib
{
    VMTDDeviceTableForm::VMTDDeviceTableForm(QWidget *parent, VMTDDeviceManager *manager, EnType type) :
        QWidget(parent),
        ui(new Ui::VMTDDeviceTableForm),
        m_manager(manager),
        m_type(type)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        m_viewModel = new QSqlQueryModel(this);

        ui->tableView->setModel(m_viewModel);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        connect(m_manager, &VMTDDeviceManager::switchesUpdatedSignal,
                this, &VMTDDeviceTableForm::switchesUpdatedSlot);
        connect(m_manager, &VMTDDeviceManager::hostsUpdatedSignal,
                this, &VMTDDeviceTableForm::hostsUpdatedSlot);

        if (m_type == EnType::SWITCHES)
        {
            m_switchDialog = new VMTDDeviceSwitchDialog(this);
            connect(m_switchDialog, &VMTDDeviceSwitchDialog::finished,
                    this, &VMTDDeviceTableForm::switchDialogFinishedSlot);
        }
        else if (m_type == EnType::HOSTS)
        {
            m_hostDialog = new VMTDDeviceHostDialog(this);
            connect(m_hostDialog, &VMTDDeviceHostDialog::finished,
                    this, &VMTDDeviceTableForm::hostDialogFinishedSlot);
        }

        on_pbLoad_clicked();
    }

    VMTDDeviceTableForm::~VMTDDeviceTableForm()
    {
        delete ui;
    }

    void VMTDDeviceTableForm::switchesUpdatedSlot(const QSqlQuery &query)
    {
        if (m_type == EnType::SWITCHES)
        {
            m_viewModel->setQuery(query);

            m_viewModel->setHeaderData(0, Qt::Horizontal, "Id");
            m_viewModel->setHeaderData(1, Qt::Horizontal, "Name");
            m_viewModel->setHeaderData(2, Qt::Horizontal, "URL");
            m_viewModel->setHeaderData(3, Qt::Horizontal, "Username");
            m_viewModel->setHeaderData(4, Qt::Horizontal, "Password");
        }
        else if (m_type == EnType::HOSTS)
        {
            QSqlQueryModel model;
            model.setQuery(query);

            QMap<int, QString> switchesMap;

            for (int i = 0; i < model.rowCount(); ++i)
            {
                const auto id = model.data(model.index(i, 0)).toInt();
                const auto switchName = model.data(model.index(i, 1)).toString();
                switchesMap[id] = switchName;
            }

            m_hostDialog->initializeView(switchesMap);
        }
    }

    void VMTDDeviceTableForm::hostsUpdatedSlot(const QSqlQuery &query)
    {
        if (m_type == EnType::HOSTS)
        {
            m_viewModel->setQuery(query);

            m_viewModel->setHeaderData(0, Qt::Horizontal, "Id");
            m_viewModel->setHeaderData(1, Qt::Horizontal, "Name");
            m_viewModel->setHeaderData(2, Qt::Horizontal, "IP");
            m_viewModel->setHeaderData(3, Qt::Horizontal, "Interface");
            m_viewModel->setHeaderData(4, Qt::Horizontal, "Switch port");
            m_viewModel->setHeaderData(5, Qt::Horizontal, "Switch name");
        }
    }

    void VMTDDeviceTableForm::switchDialogFinishedSlot(int result)
    {
        if (result == QDialog::Accepted)
        {
            m_manager->createSwitch(m_switchDialog->id(), m_switchDialog->name(),
                                    m_switchDialog->url(),
                                    m_switchDialog->username(),
                                    m_switchDialog->password());
            m_manager->selectSwitches();
        }
    }
    void VMTDDeviceTableForm::hostDialogFinishedSlot(int result)
    {
        if (result == QDialog::Accepted)
        {
            m_manager->createHost(m_hostDialog->id(), m_hostDialog->name(),
                                  m_hostDialog->ip(),
                                  m_hostDialog->interface(),
                                  m_hostDialog->switchport(),
                                  m_hostDialog->switchId());
            m_manager->selectHosts();
        }
    }

    void VMTDDeviceTableForm::on_tableView_doubleClicked(const QModelIndex &index)
    {
        if (m_type == EnType::SWITCHES)
        {
            m_switchDialog->setId(ui->tableView->model()->data(
                                      ui->tableView->model()->index(index.row(), 0)).toInt());
            m_switchDialog->setName(ui->tableView->model()->data(
                                        ui->tableView->model()->index(index.row(), 1)).toString());
            m_switchDialog->setUrl(ui->tableView->model()->data(
                                       ui->tableView->model()->index(index.row(), 2)).toString());
            m_switchDialog->setUsername(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 3)).toString());
            m_switchDialog->setPassword(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 4)).toString());
            m_switchDialog->exec();
        }
        else if (m_type == EnType::HOSTS)
        {
            m_hostDialog->setId(ui->tableView->model()->data(
                                    ui->tableView->model()->index(index.row(), 0)).toInt());
            m_hostDialog->setName(ui->tableView->model()->data(
                                      ui->tableView->model()->index(index.row(), 1)).toString());
            m_hostDialog->setIp(ui->tableView->model()->data(
                                    ui->tableView->model()->index(index.row(), 2)).toString());
            m_hostDialog->setInterface(ui->tableView->model()->data(
                                           ui->tableView->model()->index(index.row(), 3)).toString());
            m_hostDialog->setSwitchport(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 4)).toString());
            m_hostDialog->setSwitchName(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 5)).toString());
            m_hostDialog->exec();
        }
    }

    void VMTDDeviceTableForm::on_pbLoad_clicked()
    {
        m_manager->selectSwitches();
        m_manager->selectHosts();
    }

    void VMTDDeviceTableForm::on_pbCreate_clicked()
    {
        if (m_type == EnType::SWITCHES)
        {
            m_switchDialog->setId(-1);
            m_switchDialog->setName(QString());
            m_switchDialog->setUrl(QString());
            m_switchDialog->setUsername(QString());
            m_switchDialog->setPassword(QString());
            m_switchDialog->exec();
        }
        else if (m_type == EnType::HOSTS)
        {
            m_hostDialog->setId(-1);
            m_hostDialog->setName(QString());
            m_hostDialog->setIp(QString());
            m_hostDialog->setInterface(QString());
            m_hostDialog->setSwitchport(QString());
            m_hostDialog->setSwitchName(QString());
            m_hostDialog->exec();
        }
    }
    void VMTDDeviceTableForm::on_pbRemove_clicked()
    {
        QList<int> ids;

        for (int i = 0; i < ui->tableView->selectionModel()->selectedRows().count(); i++)
        {
            int row = ui->tableView->selectionModel()->selectedRows().at(i).row();
            int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
            ids.append(id);
        }

        if (m_type == EnType::SWITCHES)
        {
            for (auto id : ids)
                m_manager->removeSwitch(id);

            m_manager->selectSwitches();
        }
        else if (m_type == EnType::HOSTS)
        {
            for (auto id : ids)
                m_manager->removeHost(id);

            m_manager->selectHosts();
        }
    }
}
