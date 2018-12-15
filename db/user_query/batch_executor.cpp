#include "batch_executor.h"
#include "db/query.h"
#include "helpers/logger.h"

namespace meow {
namespace db {
namespace user_query {

BatchExecutor::BatchExecutor()
{

}

bool BatchExecutor::run(Connection * connection, const QStringList & queries)
{
    _results.clear();
    _error = db::Exception();
    bool success = true;

    for (const QString & SQL : queries) {

        db::QueryPtr query = connection->createQuery();
        query->setSQL(SQL);

        try {
            query->execute();
        } catch(meow::db::Exception & ex) {
            meowLogCC(Log::Category::Error, connection)
                << "[UserQuery] " << "Failed to run query: "
                << SQL << ex.message();
            success = false;
            _error = ex;
            break;
        }
        if (query->hasResult()) {
            _results.push_back(query);
        }
    }

    return success;
}

} // namespace user_query
} // namespace db
} // namespace meow
