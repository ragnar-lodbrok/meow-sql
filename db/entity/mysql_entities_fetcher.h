#ifndef DB_MYSQL_ENTITIES_FETCHER_H
#define DB_MYSQL_ENTITIES_FETCHER_H

#include "entities_fetcher.h"

namespace meow {
namespace db {

class MySQLConnection;

class MySQLEntitiesFetcher : public DataBaseEntitiesFetcher
{
public:
    MySQLEntitiesFetcher(MySQLConnection * connection);
    virtual void run(const QString & dbName,
             EntityListForDataBase * toList) override;
private:
    void fetchTablesViews(const QString & dbName,
                          EntityListForDataBase * toList);
    void fetchStoredFunctions(const QString & dbName,
                          EntityListForDataBase * toList);
    void fetchStoredProcedures(const QString & dbName,
                          EntityListForDataBase * toList);
    void fetchTriggers(const QString & dbName,
                       EntityListForDataBase * toList);
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_ENTITIES_FETCHER_H
