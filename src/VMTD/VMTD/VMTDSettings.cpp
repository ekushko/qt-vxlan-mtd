#include "VMTDSettings.h"
#include "VMTDSettingsForm.h"

#include "VMTDVarname.h"
#include "VMTDTemplate.h"

#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDSettings::VMTDSettings(QObject *parent, const QString &systemName)
        : QObject(parent)
        , m_systemName(systemName)
    {
        m_nodeType = EnNodeType::CLIENT;
        m_debugName = "VMTD";
        m_shouldShowDebug = true;
        m_dbName = "physical_description";

        m_shouldBeRestarted = false;

        connect(this, &VMTDSettings::saveSignal, this, &VMTDSettings::saveSlot);
        connect(this, &VMTDSettings::loadSignal, this, &VMTDSettings::loadSlot);

        if (!QDir(DIR_NAME).exists())
        {
            if (!QDir().mkdir(DIR_NAME))
            {
                debugOut(VN_S(VMTDSettings) + " | DIR creating: could not create directory " + DIR_NAME);
                return;
            }
        }
        else
        {
            load();
        }
    }

    VMTDSettings::~VMTDSettings()
    {
        if (m_form != nullptr)
            delete m_form;
    }

    void VMTDSettings::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDSettingsForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    const QString &VMTDSettings::enNodeTypeToS(const EnNodeType &nodeType)
    {
        switch (nodeType)
        {
        // *INDENT-OFF*
        case EnNodeType::CLIENT : RETURN_S("Client");
        case EnNodeType::SERVER : RETURN_S("Server");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDSettings::enNodeTypeToL()
    {
        RETURN_MAP(EnNodeType, enNodeTypeToS);
    }

    void VMTDSettings::debugOut(const QString &text)
    {
        if (m_shouldShowDebug)
            qDebug() << m_debugName +  " | " + text;
    }

    QJsonObject VMTDSettings::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_nodeType)] = static_cast<int>(m_nodeType);
        jsonObj[VN_ME(m_systemName)] = m_systemName;
        jsonObj[VN_ME(m_debugName)] = m_debugName;
        jsonObj[VN_ME(m_shouldShowDebug)] = m_shouldShowDebug;
        jsonObj[VN_ME(m_dbName)] = m_dbName;

        return jsonObj;
    }
    void VMTDSettings::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_nodeType = (EnNodeType)jsonObj[VN_ME(m_nodeType)].toInt((int)m_nodeType);
        m_debugName = jsonObj[VN_ME(m_debugName)].toString(m_debugName);
        m_shouldShowDebug = jsonObj[VN_ME(m_shouldShowDebug)].toBool(m_shouldShowDebug);
        m_dbName = jsonObj[VN_ME(m_dbName)].toString(m_dbName);
    }

    QString VMTDSettings::filePath() const
    {
        QString filePath = QString("%1%2%3%4")
                           .arg(DIR_NAME,
                                QDir::separator(),
                                m_systemName,
                                FILE_EXTENSION);

        return filePath;
    }

    void VMTDSettings::save()
    {
        emit saveSignal();
    }
    void VMTDSettings::load()
    {
        emit loadSignal();
    }

    void VMTDSettings::apply()
    {
        if (m_shouldBeRestarted)
        {
            emit restartSignal();

            m_shouldBeRestarted = false;
        }
    }

    VMTDSettings::EnNodeType VMTDSettings::nodeType() const
    {
        return m_nodeType;
    }
    void VMTDSettings::setNodeType(const VMTDSettings::EnNodeType &nodeType)
    {
        if (m_nodeType != nodeType)
        {
            m_nodeType = nodeType;

            m_shouldBeRestarted = true;
        }
    }

    QString VMTDSettings::systemName() const
    {
        return m_systemName;
    }

    QString VMTDSettings::debugName() const
    {
        return m_debugName;
    }
    void VMTDSettings::setDebugName(const QString &debugName)
    {
        m_debugName = debugName;
    }

    bool VMTDSettings::shouldShowDebug() const
    {
        return m_shouldShowDebug;
    }
    void VMTDSettings::setShouldShowDebug(bool shouldShowDebug)
    {
        m_shouldShowDebug = shouldShowDebug;
    }

    QString VMTDSettings::dbName() const
    {
        return m_dbName;
    }
    void VMTDSettings::setDbName(const QString &dbName)
    {
        m_dbName = dbName;
    }

    void VMTDSettings::saveSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.open(QIODevice::WriteOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Save: could not open settings file");
        }
        else
        {
            QJsonDocument jsonDoc;
            jsonDoc.setObject(toJson());
            settingsFile.write(jsonDoc.toJson());
            settingsFile.close();

            debugOut(VN_S(VMTDSettings) + " | Settings were saved successfully.");
        }
    }
    void VMTDSettings::loadSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.exists())
        {
            saveSlot();
            return;
        }

        if (!settingsFile.open(QIODevice::ReadOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Load: could not open settings file");
        }
        else
        {
            QByteArray settingsData = settingsFile.readAll();
            settingsFile.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(settingsData);
            fromJson(jsonDoc.object());

            debugOut(VN_S(VMTDSettings) + " | Settings were loaded successfully.");
        }
    }
}
