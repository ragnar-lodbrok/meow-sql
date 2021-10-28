#include "batch_executor.h"
#include "db/query.h"
#include "helpers/logger.h"

namespace meow {
namespace db {
namespace user_query {

BatchExecutor::BatchExecutor() : _failed(false)
{

}

bool BatchExecutor::run(Connection * connection, const QStringList & queries)
{
    _results.clear();
    _error = db::Exception();
    _failed = false;

    for (const QString & SQL : queries) {

        db::QueryPtr query = connection->createQuery();
        query->setSQL(SQL);

        try {
            query->execute();
        } catch(meow::db::Exception & ex) {
            meowLogDebugC(connection)
                << "[UserQuery] " << "Failed to run query: "
                << SQL << ex.message();
            _failed = true;
            _error = ex;
            break;
        }
        if (query->hasResult()) {
            _results.push_back(query);
        }
    }

    return !_failed;
}

} // namespace user_query
} // namespace db
} // namespace meow
