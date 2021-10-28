#include "db_thread_initializer.h"
#include "connection_parameters.h" // enum class ServerType

#ifdef WITH_MYSQL

#include "mysql/mysql_thread_initializer.h"

#endif

namespace meow {
namespace db {

std::unique_ptr<DbThreadInitializer> createThreadInitializer(
        ServerType serverType)
{
    std::unique_ptr<DbThreadInitializer> init;

    switch (serverType) {
#ifdef WITH_MYSQL
    case ServerType::MySQL:
        init.reset(new MySQLThreadInitializer);
        break;
#endif
    default:
        init.reset(new DbThreadInitializer);
        break;
    }

    return init;
}

} // namespace db
} // namespace meow
