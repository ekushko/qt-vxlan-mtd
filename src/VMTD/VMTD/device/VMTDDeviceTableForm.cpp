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
        connect(m_manager, &VMTDDeviceManager::bannedScannersUpdatedSignal,
                this, &VMTDDeviceTableForm::bannedScannersUpdatedSlot);
        connect(m_manager, &VMTDDeviceManager::allowedScannersUpdatedSignal,
                this, &VMTDDeviceTableForm::allowedScannersUpdatedSlot);

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
        else if (m_type == EnType::BANNED_SCANNERS)
        {
            m_bannedScannerDialog = new VMTDDeviceScannerDialog(this);
            connect(m_bannedScannerDialog, &VMTDDeviceScannerDialog::finished,
                    this, &VMTDDeviceTableForm::bannedScannerDialogFinishedSlot);
        }
        else if (m_type == EnType::ALLOWED_SCANNERS)
        {
            m_allowedScannerDialog = new VMTDDeviceScannerDialog(this);
            connect(m_allowedScannerDialog, &VMTDDeviceScannerDialog::finished,
                    this, &VMTDDeviceTableForm::allowedScannerDialogFinishedSlot);
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
            m_viewModel->setHeaderData(2, Qt::Horizontal, "Domain name");
            m_viewModel->setHeaderData(3, Qt::Horizontal, "IP");
            m_viewModel->setHeaderData(4, Qt::Horizontal, "Interface");
            m_viewModel->setHeaderData(5, Qt::Horizontal, "Switch port");
            m_viewModel->setHeaderData(6, Qt::Horizontal, "Switch name");
        }
        else if (m_type == EnType::BANNED_SCANNERS
                 || m_type == EnType::ALLOWED_SCANNERS)
        {
            QSqlQueryModel model;
            model.setQuery(query);

            QMap<int, QString> hostsMap;
            QMap<int, QString> hostIpsMap;

            for (int i = 0; i < model.rowCount(); ++i)
            {
                const auto id = model.data(model.index(i, 0)).toInt();
                const auto hostName = model.data(model.index(i, 1)).toString();
                const auto hostIp = model.data(model.index(i, 3)).toString();
                hostsMap[id] = hostName;
                hostIpsMap[id] = hostIp;
            }

            if (m_type == EnType::BANNED_SCANNERS)
                m_bannedScannerDialog->initializeView(VMTDDeviceScannerDialog::EnType::BANNED,
                                                      hostsMap, hostIpsMap);

            if (m_type == EnType::ALLOWED_SCANNERS)
                m_allowedScannerDialog->initializeView(VMTDDeviceScannerDialog::EnType::ALLOWED,
                                                       hostsMap, hostIpsMap);
        }
    }
    void VMTDDeviceTableForm::bannedScannersUpdatedSlot(const QSqlQuery &query)
    {
        if (m_type == EnType::BANNED_SCANNERS)
        {
            m_viewModel->setQuery(query);

            m_viewModel->setHeaderData(0, Qt::Horizontal, "Id");
            m_viewModel->setHeaderData(1, Qt::Horizontal, "Host");
            m_viewModel->setHeaderData(2, Qt::Horizontal, "Remaining");
        }
    }
    void VMTDDeviceTableForm::allowedScannersUpdatedSlot(const QSqlQuery &query)
    {
        if (m_type == EnType::ALLOWED_SCANNERS)
        {
            m_viewModel->setQuery(query);

            m_viewModel->setHeaderData(0, Qt::Horizontal, "Id");
            m_viewModel->setHeaderData(1, Qt::Horizontal, "Host");
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
            m_manager->createHost(m_hostDialog->id(),
                                  m_hostDialog->name(),
                                  m_hostDialog->domainName(),
                                  m_hostDialog->ip(),
                                  m_hostDialog->interface(),
                                  m_hostDialog->switchport(),
                                  m_hostDialog->switchId());
            m_manager->selectHosts();
        }
    }
    void VMTDDeviceTableForm::bannedScannerDialogFinishedSlot(int result)
    {
        if (result == QDialog::Accepted)
        {
            m_manager->createBannedScanner(m_bannedScannerDialog->id(),
                                           m_bannedScannerDialog->hostId(),
                                           m_bannedScannerDialog->remaining());
            m_manager->selectBannedScanners();
        }
    }
    void VMTDDeviceTableForm::allowedScannerDialogFinishedSlot(int result)
    {
        if (result == QDialog::Accepted)
        {
            m_manager->createAllowedScanner(m_allowedScannerDialog->id(),
                                            m_allowedScannerDialog->hostId());
            m_manager->selectAllowedScanners();
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
            m_hostDialog->setDomainName(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 2)).toString());
            m_hostDialog->setIp(ui->tableView->model()->data(
                                    ui->tableView->model()->index(index.row(), 3)).toString());
            m_hostDialog->setInterface(ui->tableView->model()->data(
                                           ui->tableView->model()->index(index.row(), 4)).toString());
            m_hostDialog->setSwitchport(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 5)).toString());
            m_hostDialog->setSwitchName(ui->tableView->model()->data(
                                            ui->tableView->model()->index(index.row(), 6)).toString());
            m_hostDialog->exec();
        }
        else if (m_type == EnType::BANNED_SCANNERS)
        {
            m_bannedScannerDialog->setId(ui->tableView->model()->data(
                                             ui->tableView->model()->index(index.row(), 0)).toInt());
            m_bannedScannerDialog->setHostName(ui->tableView->model()->data(
                                                   ui->tableView->model()->index(index.row(), 1)).toString());
            m_bannedScannerDialog->setRemaining(ui->tableView->model()->data(
                                                    ui->tableView->model()->index(index.row(), 2)).toInt());
            m_bannedScannerDialog->exec();
        }
        else if (m_type == EnType::ALLOWED_SCANNERS)
        {
            m_allowedScannerDialog->setId(ui->tableView->model()->data(
                                              ui->tableView->model()->index(index.row(), 0)).toInt());
            m_allowedScannerDialog->setHostName(ui->tableView->model()->data(
                                                    ui->tableView->model()->index(index.row(), 1)).toString());
            m_allowedScannerDialog->exec();
        }
    }

    void VMTDDeviceTableForm::on_pbApply_clicked()
    {
        m_manager->apply();
    }
    void VMTDDeviceTableForm::on_pbLoad_clicked()
    {
        m_manager->selectSwitches();
        m_manager->selectHosts();
        m_manager->selectBannedScanners();
        m_manager->selectAllowedScanners();
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
            m_hostDialog->setDomainName(QString());
            m_hostDialog->setIp(QString());
            m_hostDialog->setInterface(QString());
            m_hostDialog->setSwitchport(QString());
            m_hostDialog->setSwitchName(QString());
            m_hostDialog->exec();
        }
        else if (m_type == EnType::BANNED_SCANNERS)
        {
            m_bannedScannerDialog->setId(-1);
            m_bannedScannerDialog->setHostName(QString());
            m_bannedScannerDialog->setRemaining(m_manager->settings()->exclusionInterval());
            m_bannedScannerDialog->exec();
        }
        else if (m_type == EnType::ALLOWED_SCANNERS)
        {
            m_allowedScannerDialog->setId(-1);
            m_allowedScannerDialog->setHostName(QString());
            m_allowedScannerDialog->setRemaining(-1);
            m_allowedScannerDialog->exec();
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
        else if (m_type == EnType::BANNED_SCANNERS)
        {
            for (auto id : ids)
                m_manager->removeBannedScanner(id);

            m_manager->selectBannedScanners();
        }
        else if (m_type == EnType::ALLOWED_SCANNERS)
        {
            for (auto id : ids)
                m_manager->removeAllowedScanner(id);

            m_manager->selectAllowedScanners();
        }
    }
}
