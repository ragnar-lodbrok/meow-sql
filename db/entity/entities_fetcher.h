#ifndef DATABASE_ENTITIES_FETCHER_H
#define DATABASE_ENTITIES_FETCHER_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class EntityListForDataBase;

class DataBaseEntitiesFetcher
{
public:
    explicit DataBaseEntitiesFetcher(Connection * connection);
    virtual ~DataBaseEntitiesFetcher() {}
    virtual void run(const QString & dbName,
             EntityListForDataBase * toList) = 0;
protected:  
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DATABASE_ENTITIES_FETCHER_H
