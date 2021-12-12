#include "queries_task.h"


namespace meow {
namespace threads {

QueriesTask::QueriesTask(const db::SQLBatch & queries, db::Connection * connection)
    : ThreadTask(TaskType::Query)
    , _queries(queries)
    , _connection(connection)
{
    connect(&_executor, &db::user_query::BatchExecutor::afterQueryExecution,
            this, &QueriesTask::queryFinished);
}

QueriesTask::~QueriesTask()
{

}

void QueriesTask::run()
{
    _executor.run(_connection, _queries);
    emit finished();
    if (isFailed()) {
        emit failed();
    }
}

bool QueriesTask::isFailed() const
{
    return _executor.failed();
}

QString QueriesTask::errorMessage() const
{
    return _executor.error().message();
}

int QueriesTask::currentResultsCount() const
{
    return _executor.currentQueryIndex() + 1;
}

db::QueryPtr QueriesTask::resultAt(int queryIndex) const
{
    return _executor.resultAt(queryIndex);
}

} // namespace threads
} // namespace meow
