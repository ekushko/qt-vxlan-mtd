#pragma once

#include <QObject>

class VMTDClient : public QObject
{
    Q_OBJECT
public:
    explicit VMTDClient(QObject *parent = nullptr);

signals:

};

