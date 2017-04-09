#include "formatting.h"
#include <QStringList>

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

} // namespace helpers
} // namespace meow

