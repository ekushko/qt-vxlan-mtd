#include "VMTDBuildInfo.h"

#include <QLocale>

namespace VMTDLib
{
    QString VMTDBuildInfo::version()
    {
        return QString(VERSION);
    }

    QString VMTDBuildInfo::filename()
    {
        return QString(TARGET);
    }

    QDateTime VMTDBuildInfo::dateTime()
    {
        QLocale loc = QLocale(QLocale::English);

        QString date = QString(__DATE__);
        date.replace("  ", " ");

        QDate compileDate = loc.toDate(date, "MMM d yyyy");

        QTime compileTime = QTime::fromString(__TIME__, "hh:mm:ss");

        return QDateTime(compileDate, compileTime);
    }
}
