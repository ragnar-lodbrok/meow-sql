#include "pg_entities_fetcher.h"
#include "db/pg/pg_connection.h"

namespace meow {
namespace db {


PGEntitiesFetcher::PGEntitiesFetcher(PGConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void PGEntitiesFetcher::run(const QString & dbName,
                            EntityListForDataBase * toList)
{
    Q_UNUSED(dbName);
    Q_UNUSED(toList);

    // TODO
    // Empty impl added to avoid crash on expanding session's entities
}

} // namespace db
} // namespace meow
