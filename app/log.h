#ifndef MEOW_LOG_H
#define MEOW_LOG_H

#include <QString>

namespace meow {

namespace db {
    class Connection;
}


class Log
{
public:

    enum class Category
    {
        Info,
        SQL,
        UserSQL,
        Error,
        Debug
    };

    Log();

    void message(const QString & msg,
                 Category category = Category::Debug,
                 const db::Connection * connection = nullptr) const;
};

} // namespace meow

#endif // MEOW_LOG_H
