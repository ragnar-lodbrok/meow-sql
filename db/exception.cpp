#include "exception.h"

namespace meow {
namespace db {

Exception::Exception(const QString & message, unsigned int code)
:
    std::runtime_error(message.toStdString()),
    _message(message),
    _code(code)
{

}

} // namespace db
} // namespace meow

