#ifndef DB_ENTITY_FILTER_H
#define DB_ENTITY_FILTER_H

#include <QList>

namespace meow {
namespace db {

class Connection;
class TableEntity;
class EntityListForDataBase;

class EntityFilter
{
public:
    EntityFilter(Connection * connection);
    virtual QList<TableEntity *> tablesWithForeignKeySupport(
            const QString & dbName) const = 0;
    TableEntity * tableByName(
            const QString & databaseName,
            const QString & tableName) const;
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_FILTER_H
