#include "parsing.h"
#include <QDebug>

namespace meow {
namespace helpers {

QDateTime parseDateTime(const QString & dateTime)
{
    QString dt = dateTime;

    // The fractional part should always be separated from the rest of the time
    // by a decimal point; no other fractional seconds delimiter is recognized.
    int indexOfDot = dateTime.indexOf(".");
    if (indexOfDot != -1) {
        dt = dateTime.left(indexOfDot);
    }

    // example: "2017-05-29 21:27:25"
    return QDateTime::fromString(dt, "yyyy-MM-dd HH:mm:ss");
}

} // namespace helpers
} // namespace meow
