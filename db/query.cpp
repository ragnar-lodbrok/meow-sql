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

void Query::seekFirst()
{
    _curRecNo = -1;
    seekRecNo(0);
}

void Query::seekNext()
{
    seekRecNo(_curRecNo + 1);
}

} // namespace db
} // namespace meow

