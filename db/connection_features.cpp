#include "connection_features.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace db {


ConnectionFeatures::ConnectionFeatures(Connection * connection)
{
    (void)(connection);
}

ConnectionFeatures::~ConnectionFeatures()
{

}

////////////////////////////////////////////////////////////////////////////////

MySQLConnectionFeatures::MySQLConnectionFeatures(Connection * connection)
    : ConnectionFeatures(connection)
{

}

bool MySQLConnectionFeatures::supportsForeignKeys(
        const TableEntity * table) const
{
    return table->engineStr().toLower() == "innodb"; // TODO: ndb
}

////////////////////////////////////////////////////////////////////////////////

PGConnectionFeatures::PGConnectionFeatures(Connection * connection)
    : ConnectionFeatures(connection)
{

}

////////////////////////////////////////////////////////////////////////////////

QtSQLiteConnectionFeatures::QtSQLiteConnectionFeatures(Connection * connection)
    : ConnectionFeatures(connection)
{

}

} // namespace db
} // namespace meow
