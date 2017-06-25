#ifndef FORMATTING_H
#define FORMATTING_H

#include <QString>
#include <QDateTime>

namespace meow {
namespace helpers {

typedef unsigned long long byteSize;

QString formatByteSize(byteSize bytes, int decimals = 1);
QString formatNumber(unsigned long long number);
QString formatDateTime(const QDateTime & dateTime);

} // namespace helpers
} // namespace meow

#endif // FORMATTING_H
