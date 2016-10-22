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

} // namespace db
} // namespace meow

