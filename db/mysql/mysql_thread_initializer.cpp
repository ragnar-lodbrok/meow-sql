#include "mysql_thread_initializer.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

// https://dev.mysql.com/doc/c-api/5.7/en/c-api-threaded-clients.html

namespace meow {
namespace db {

MySQLThreadInitializer::MySQLThreadInitializer(bool deinitOnDestroy)
    : DbThreadInitializer(deinitOnDestroy)
{

}

void MySQLThreadInitializer::init()
{
    if (!_initialized) {
        _initialized = (mysql_thread_init() != 0);
    }
}

void MySQLThreadInitializer::deinit()
{
    if (_initialized) {
        mysql_thread_end();
        _initialized = false;
    }
}

} // namespace db
} // namespace meow
