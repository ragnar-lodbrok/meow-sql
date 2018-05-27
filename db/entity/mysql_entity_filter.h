#ifndef DB_MYSQL_ENTITY_FILTER_H
#define DB_MYSQL_ENTITY_FILTER_H

#include "entity_filter.h"

namespace meow {
namespace db {

class MySQLConnection;

class MySQLEntityFilter : public EntityFilter
{
public:
    MySQLEntityFilter(MySQLConnection * connection);
    virtual QList<TableEntity *> tablesWithForeignKeySupport(
        const QString & dbName) const override;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_ENTITY_FILTER_H
