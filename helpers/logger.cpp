#include "logger.h"
#include "app/app.h"

namespace meow {
namespace helpers {

LogWrapper::LogWrapper(Log::Category category,
                       const db::Connection * connection)
    :_category(category),
     _connection(connection)
{
    _stream.setString(&_buffer);
}

void LogWrapper::flush()
{
    QString msg = _stream.readAll();
    meow::app()->log()->message(msg, _category, _connection);
}

} // namespace helpers
} // namespace meow
