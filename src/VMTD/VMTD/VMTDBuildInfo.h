#pragma once

#include <QDateTime>

#include "VMTDDeclareShared.h"

namespace VMTDLib
{
    class VMTD_SHARED VMTDBuildInfo
    {
        Q_DISABLE_COPY(VMTDBuildInfo)

    public:

        VMTDBuildInfo() = delete;

        static QString version();

        static QString filename();

        static QDateTime dateTime();
    };
}
