#ifndef MEOW_DB_MYSQL_THREAD_INITIALIZER_H
#define MEOW_DB_MYSQL_THREAD_INITIALIZER_H

#include "db/db_thread_initializer.h"

namespace meow {
namespace db {

class MySQLThreadInitializer : public DbThreadInitializer
{
public:
    MySQLThreadInitializer(bool deinitOnDestroy = false);

    ~MySQLThreadInitializer() override {}

    void init() override;
    void deinit() override;
};

} // namespace db
} // namespace meow

#endif // MEOW_DB_MYSQL_THREAD_INITIALIZER_H
