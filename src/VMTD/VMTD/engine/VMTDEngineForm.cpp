#include    "VMTDEngineForm.h"
#include "ui_VMTDEngineForm.h"

namespace VMTDLib
{
    VMTDEngineForm::VMTDEngineForm(QWidget *parent, VMTDEngine *engine) :
        QWidget(parent),
        ui(new Ui::VMTDEngineForm),
        m_engine(engine)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        updateView();
    }

    VMTDEngineForm::~VMTDEngineForm()
    {
        delete ui;
    }

    void VMTDEngineForm::updateView()
    {
        ui->treeWidget->clear();

        for (auto group : m_engine->groups())
        {
            // группа

            auto groupItem = new QTreeWidgetItem(ui->treeWidget);

            const auto groupLabel = QString("Group-%1 (%2/%3 VLAN: %4)")
                                    .arg(group->index())
                                    .arg(group->network())
                                    .arg(group->mask())
                                    .arg(group->vlanId());
            groupItem->setText(0, groupLabel);

            // участники

            auto participantsItem = new QTreeWidgetItem(groupItem);

            const auto participantsLabel = QString("Participants");
            participantsItem->setText(0, participantsLabel);

            for (auto participant : group->participants())
            {
                // физическое подключение

                auto participantItem = new QTreeWidgetItem(participantsItem);

                const auto participantLabel = QString("%1 (%2 %3 connected to %4 %5)")
                                              .arg(participant->hostName())
                                              .arg(participant->hostIp())
                                              .arg(participant->hostInterface())
                                              .arg(participant->switchPort())
                                              .arg(participant->switchName());
                participantItem->setText(0, participantLabel);

                fillInterface(participant->interface1(), participantItem);

                if (participant->role() == VMTDParticipant::EnRole::GATEWAY
                    && participant->interface2()->isExist())
                    fillInterface(participant->interface2(), participantItem);
            }
        }
    }

    void VMTDEngineForm::fillInterface(VMTDInterface *interface, QTreeWidgetItem *item)
    {
        // интерфейс

        auto interfaceItem = new QTreeWidgetItem(item);

        const auto interfaceLabel = QString("Network interface");
        interfaceItem->setText(0, interfaceLabel);

        // mac

        auto macAddressItem = new QTreeWidgetItem(interfaceItem);

        const auto macAddressLabel = QString("MAC Address: %1")
                                     .arg(interface->mac());
        macAddressItem->setText(0, macAddressLabel);

        // ip

        auto ipAddressItem = new QTreeWidgetItem(interfaceItem);

        const auto ipAddressLabel = QString("IP Address: %1/%2")
                                    .arg(interface->ip())
                                    .arg(interface->mask());
        ipAddressItem->setText(0, ipAddressLabel);

        // vlanId

        auto vlanItem = new QTreeWidgetItem(interfaceItem);

        const auto vlanLabel = QString("VLAN: %1")
                               .arg(interface->vlanId());
        vlanItem->setText(0, vlanLabel);

        // маршруты

        if (!interface->routes().isEmpty())
        {
            auto routesItem = new QTreeWidgetItem(interfaceItem);

            const auto routesLabel = QString("Routes");
            routesItem->setText(0, routesLabel);

            for (auto route : interface->routes())
            {
                auto routeItem = new QTreeWidgetItem(routesItem);

                const auto routeLabel = QString("to %1/%2 via %3 metric %4")
                                        .arg(route.network())
                                        .arg(route.mask())
                                        .arg(route.gateway())
                                        .arg(route.metric());
                routeItem->setText(0, routeLabel);
            }
        }
    }

    void VMTDEngineForm::on_pbExecute_clicked()
    {
        m_engine->execute();

        updateView();
    }
}
