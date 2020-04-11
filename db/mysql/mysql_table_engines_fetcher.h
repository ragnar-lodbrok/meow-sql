#ifndef MYSQL_TABLE_ENGINES_FETCHER_H
#define MYSQL_TABLE_ENGINES_FETCHER_H

#include "db/table_engines_fetcher.h"

namespace meow {
namespace db {

class MySQLTableEnginesFetcher : public TableEnginesFetcher
{
public:
    MySQLTableEnginesFetcher(Connection * connection);
    virtual QStringList run() override;
};

} // namespace db
} // namespace meow

#endif // MYSQL_TABLE_ENGINES_FETCHER_H
