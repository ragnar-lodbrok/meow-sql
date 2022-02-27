#ifndef PARSING_H
#define PARSING_H

#include <QString>
#include <QDateTime>

namespace meow {
namespace helpers {

QDateTime parseDateTime(const QString & dateTime);
QDate parseDate(const QString & dateTime);
QTime parseTime(const QString & dateTime);
QDate parseYear(const QString & dateTime);

} // namespace helpers
} // namespace meow

#endif // PARSING_H
