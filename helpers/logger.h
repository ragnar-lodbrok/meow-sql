#ifndef MEOW_HELPERS_LOGGER_H
#define MEOW_HELPERS_LOGGER_H

#include <QTextStream>

#include "app/log.h"

namespace meow {
namespace helpers {

class LogWrapper
{
public:
    LogWrapper(Log::Category category = Log::Category::Debug,
               const db::Connection * connection = nullptr);
    ~LogWrapper() { flush(); }

    QTextStream& stream() { return _stream; }

private:

    void flush();

    Log::Category _category;
    const db::Connection * _connection;
    QString _buffer;
    QTextStream _stream;
};

} // namespace helpers
} // namespace meow

#define meowLog() meow::helpers::LogWrapper()

#define meowLogDebug() \
meow::helpers::LogWrapper(meow::Log::Category::Debug).stream()

#define meowLogDebugC(connection) \
meow::helpers::LogWrapper(meow::Log::Category::Debug, (connection)).stream()

#define meowLogC(category) meow::helpers::LogWrapper((category)).stream()

#define meowLogCC(category, connection) \
meow::helpers::LogWrapper((category), (connection)).stream()

#endif // MEOW_HELPERS_LOGGER_H
