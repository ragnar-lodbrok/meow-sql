#ifndef DB_EXCEPTION_H
#define DB_EXCEPTION_H

#include <stdexcept>
#include <QString>

namespace meow {
namespace db {

class Exception : public std::runtime_error
{
public:
    Exception(const QString & message = "", unsigned int code = 0);
    const QString & message() const { return _message; }
    unsigned int code() const { return _code; }
private:
    QString _message;
    unsigned int _code;
};

} // namespace db
} // namespace meow

#endif // DB_EXCEPTION_H
