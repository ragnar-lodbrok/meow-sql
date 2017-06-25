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
    return QLocale().toString(dateTime, "yyyy-MM-dd HH:mm:ss");
}

} // namespace helpers
} // namespace meow

