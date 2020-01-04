#ifndef DB_QTSQL_ENTITIES_FETCHER_H
#define DB_QTSQL_ENTITIES_FETCHER_H

#include "db/entity/entities_fetcher.h"

namespace meow {
namespace db {

class SQLiteConnection;

class SQLiteEntitiesFetcher : public DataBaseEntitiesFetcher
{
public:
    explicit SQLiteEntitiesFetcher(SQLiteConnection * connection);

    virtual void run(const QString & dbName,
                     EntityListForDataBase * toList) override;

};

} // namespace db
} // namespace meow

#endif // DB_QTSQL_ENTITIES_FETCHER_H
