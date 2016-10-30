#include "query.h"

namespace meow {
namespace db {

Query::Query(Connection * connection)
    :_connection(connection)
{

}

Query::~Query()
{

}

void Query::setSQL(const QString & SQL)
{
    _SQL = SQL;
}

} // namespace db
} // namespace meow

