#ifndef MEOW_DB_MYSQL_LIBRARY_INITIALIZER_H
#define MEOW_DB_MYSQL_LIBRARY_INITIALIZER_H

#include <mutex>

namespace meow {
namespace db {

// Intent: performs thread-safe mysql library init/deinit
class MySQLLibraryInitializer
{
public:
    MySQLLibraryInitializer(bool deinitOnDestroy = false);
    ~MySQLLibraryInitializer();

    // safe to call multiple times
    bool init();

    void deinit();

private:
    std::mutex _mutex;
    bool _initialized;
    bool _deinitOnDestroy;
};

} // namespace db
} // namespace meow

#endif // MEOW_DB_MYSQL_LIBRARY_INITIALIZER_H
