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
QString formatDate(const QDateTime & dateTime);
QString formatTime(const QDateTime & dateTime);
QString formatYear(const QDateTime & dateTime);
QString formatAsHex(const QString & str);

} // namespace helpers
} // namespace meow

#endif // FORMATTING_H
