#include "mysql_thread_initializer.h"
#include <mysql/mysql.h>

// https://dev.mysql.com/doc/c-api/5.7/en/c-api-threaded-clients.html

namespace meow {
namespace db {

MySQLThreadInitializer::MySQLThreadInitializer(bool deinitOnDestroy)
    : DbThreadInitializer(deinitOnDestroy)
    , _initialized(false)
{

}

void MySQLThreadInitializer::init()
{
    if (!_initialized) {
        _initialized = mysql_thread_init();
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
