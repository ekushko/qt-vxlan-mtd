#pragma once

#include "../device/VMTDDeviceManager.h"

#include "VMTDGroup.h"

namespace VMTDLib
{
    class VMTDEngineForm;

    class VMTDEngine : public QObject
    {
        Q_OBJECT

    public:

        VMTDEngine(QObject *parent,
                   VMTDSettings *settings,
                   VMTDDeviceManager *manager);
        ~VMTDEngine();

        QWidget *form() const;
        void showForm(QWidget *parent = nullptr);

        // ЛОГИКА

        void   startEngine();
        void    stopEngine();
        void   resetEngine();

        void generate();

        void run();

        // ДАННЫЕ

        int remainingTime() const;

        const QList<VMTDGroup *> &groups() const;

    signals:

        void appendRequestListSignal(const QString &ip, const RequestList &requests);
        void appendCommandListSignal(const QString &url, const CommandList &commands);

    public slots:

        void handleResultSlot(const QJsonValue &result);

    private:

        void createGroups();
        void createGateways();
        void createRequests();
        void createCommands();

        QJsonObject hosts() const;

        QPointer<VMTDEngineForm> m_form;

        VMTDSettings *m_settings;
        VMTDDeviceManager *m_manager;

        QList<VMTDGroup *> m_groups;

        QTimer m_reconfigTimer;

        int m_currentParticipantIndex;
        QTimer m_alertCollectTimer;

    private slots:

        void reconfigTimerTickSlot();

        void alertCollectTimerTickSlot();
    };
}
