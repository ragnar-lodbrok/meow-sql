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

bool MySQLConnectionFeatures::supportsForeignKeys(
        const TableEntity * table) const
{
    return table->engineStr().toLower() == "innodb"; // TODO: ndb
}


} // namespace db
} // namespace meow
