#ifndef DB_MYSQL_ENTITIES_FETCHER_H
#define DB_MYSQL_ENTITIES_FETCHER_H

#include "db/entity/entities_fetcher.h"

namespace meow {
namespace db {

class MySQLConnection;

class MySQLEntitiesFetcher : public DataBaseEntitiesFetcher
{
public:
    MySQLEntitiesFetcher(MySQLConnection * connection);
    virtual QList<EntityPtr> run(const QString & dbName) override;
private:
    void fetchTablesViews(const QString & dbName,
                          QList<EntityPtr> * toList);
    void fetchStoredFunctions(const QString & dbName,
                          QList<EntityPtr> * toList);
    void fetchStoredProcedures(const QString & dbName,
                          QList<EntityPtr> * toList);
    void fetchTriggers(const QString & dbName,
                       QList<EntityPtr> * toList);
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_ENTITIES_FETCHER_H
