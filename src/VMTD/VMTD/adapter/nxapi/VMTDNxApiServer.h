#pragma once

#include "../../device/VMTDDeviceManager.h"
#include "VMTDNxApiAdapter.h"

#include <QNetworkAccessManager>

namespace VMTDLib
{
    class VMTDNxApiServerForm;

    class VMTDNxApiServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDNxApiServer(QObject *parent,
                        VMTDSettings *settings,
                        VMTDDeviceManager *manager);
        ~VMTDNxApiServer();

        void showForm();

        bool isListening() const;

        const QVector<VMTDNxApiAdapter *> &adapters() const;

    signals:

        void adapterCreatedSignal(VMTDNxApiAdapter *adapter);
        void adapterRemovedSignal(VMTDNxApiAdapter *adapter);

    public slots:

        void showFormSlot();

        void   startListenSlot();
        void    stopListenSlot();
        void restartListenSlot();

    private:

        QPointer<VMTDNxApiServerForm> m_form;

        VMTDSettings *m_settings;

        VMTDDeviceManager *m_manager;

        QNetworkAccessManager *m_netManager;

        QVector<VMTDNxApiAdapter *> m_adapters;

        bool m_isListening = false;
    };
}
