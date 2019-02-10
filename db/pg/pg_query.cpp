#include "pg_query.h"

namespace meow {
namespace db {

PGQuery::PGQuery(Connection *connection)
    :Query(connection)
{

}

PGQuery::~PGQuery()
{

}

void PGQuery::execute(bool addResult)
{
    Q_ASSERT(false);
    Q_UNUSED(addResult);
}

bool PGQuery::hasResult()
{
    Q_ASSERT(false);
    return false;
}

void PGQuery::seekRecNo(db::ulonglong value)
{
    Q_UNUSED(value);
    Q_ASSERT(false);
}

QString PGQuery::curRowColumn(std::size_t index,
                             bool ignoreErrors)
{
    Q_UNUSED(index);
    Q_UNUSED(ignoreErrors);
    Q_ASSERT(false);
}

bool PGQuery::isNull(std::size_t index)
{
    Q_UNUSED(index);
    Q_ASSERT(false);
}

bool PGQuery::prepareEditing()
{
    Q_ASSERT(false);
    return false;
}

} // namespace db
} // namespace meow
