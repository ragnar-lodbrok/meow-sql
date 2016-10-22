#include "mysql_query.h"

namespace meow {
namespace db {

MySQLQuery::MySQLQuery(Connection * connection)
    :Query(connection)
{

}

void MySQLQuery::execute(bool addResult /*= false*/, int useRawResult /*= -1*/) // override
{

}

MySQLQuery::~MySQLQuery()
{

}

} // namespace db
} // namespace meow

