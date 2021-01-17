#ifndef FORMATTING_H
#define FORMATTING_H

#include <QString>
#include <QDateTime>

namespace meow {
namespace helpers {

using byteSize = unsigned long long ;

QString formatByteSize(byteSize bytes, int decimals = 1);
QString formatNumber(unsigned long long number);
QString formatDateTime(const QDateTime & dateTime);
QString formatDate(const QDateTime & dateTime);
QString formatTime(const QDateTime & dateTime);
QString formatYear(const QDateTime & dateTime);
QString formatAsHex(const QString & str);

QString dateTimeFormatString();
QString dateFormatString();
QString timeFormatString();
QString yearFormatString();

} // namespace helpers
} // namespace meow

#endif // FORMATTING_H
