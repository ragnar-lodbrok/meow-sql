#include "mysql_library_initializer.h"

#include <mysql/mysql.h>

// https://dev.mysql.com/doc/c-api/5.7/en/c-api-threaded-clients.html

namespace meow {
namespace db {

MySQLLibraryInitializer::MySQLLibraryInitializer(bool deinitOnDestroy)
    : _initialized(false)
    , _deinitOnDestroy(deinitOnDestroy)
{

}

MySQLLibraryInitializer::~MySQLLibraryInitializer()
{
    if (_deinitOnDestroy) {
        deinit();
    }
}

bool MySQLLibraryInitializer::init()
{
    std::lock_guard<std::mutex> guard(_mutex);
    if (_initialized) return true;

    // Call mysql_library_init() before any other MySQL functions.
    // It is not thread-safe, so call it before threads are created,
    // or protect the call with a mutex.

    if (mysql_library_init(0, NULL, NULL)) {
        return false;
    }

    _initialized = true;
    return true;
}

void MySQLLibraryInitializer::deinit()
{
    std::lock_guard<std::mutex> guard(_mutex);
    if (_initialized) {
        mysql_library_end();
        _initialized = false;
    }
}

} // namespace db
} // namespace meow
