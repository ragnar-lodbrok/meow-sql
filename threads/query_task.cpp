#include "query_task.h"


namespace meow {
namespace threads {

QueryTask::QueryTask(const db::SQLBatch & queries, db::Connection * connection)
    : ThreadTask(TaskType::Query)
    , _queries(queries)
    , _connection(connection)
{

}

QueryTask::~QueryTask()
{

}

void QueryTask::run()
{
    _executor.run(_connection, _queries);
    emit finished();
    if (isFailed()) {
        emit failed();
    }
}

bool QueryTask::isFailed() const
{
    return _executor.failed();
}

QString QueryTask::errorMessage() const
{
    return _executor.error().message();
}

const QList<db::QueryPtr> QueryTask::results() const
{
    return _executor.results();
}

} // namespace threads
} // namespace meow
