#ifndef DATABASE_ENTITIES_FETCHER_H
#define DATABASE_ENTITIES_FETCHER_H

#include <QString>
#include "db/entity/entity.h"

namespace meow {
namespace db {

class Connection;

class DataBaseEntitiesFetcher
{
public:
    explicit DataBaseEntitiesFetcher(Connection * connection);
    virtual ~DataBaseEntitiesFetcher() {}
    virtual QList<EntityPtr> run(const QString & dbName) = 0;
protected:  
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DATABASE_ENTITIES_FETCHER_H
