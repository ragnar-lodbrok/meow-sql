#include "formatting.h"
#include <QStringList>
#include <QLocale>

namespace meow {
namespace helpers {

QString formatByteSize(byteSize bytes, int decimals)
{

    double num = bytes;

    QStringList list;
    list << "KiB" << "MiB" << "GiB" << "TiB" << "PiB";

    QStringListIterator i(list);
    QString unit("B");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num,'f', decimals) + " " + unit;
}

QString formatNumber(unsigned long long number)
{
    return QLocale().toString(number);
}

QString formatDateTime(const QDateTime & dateTime)
{
    return dateTime.toString("yyyy-MM-dd HH:mm:ss");
}

QString formatDate(const QDateTime & dateTime)
{
    return dateTime.toString("yyyy-MM-dd");
}

QString formatTime(const QDateTime & dateTime)
{
    return dateTime.toString("HH:mm:ss");
}

QString formatYear(const QDateTime & dateTime)
{
    return dateTime.toString("yyyy");
}

QString formatAsHex(const QString & str)
{
    return QString::fromLatin1( str.toLatin1().toHex() ).toUpper();
}

} // namespace helpers
} // namespace meow

