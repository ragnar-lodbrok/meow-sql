#ifndef MEOW_THREADS_QUERY_TASK_H
#define MEOW_THREADS_QUERY_TASK_H

#include <QStringList>
#include "thread_task.h"
#include "db/user_query/batch_executor.h"
#include "db/query.h"

namespace meow {

namespace db {
using SQLBatch = QStringList;
class Connection;
}



namespace threads {

class QueriesTask : public ThreadTask
{
    Q_OBJECT
public:
    QueriesTask(const db::SQLBatch & queries, db::Connection * connection);
    ~QueriesTask() override;
    void run() override;
    bool isFailed() const override;
    QString errorMessage() const;

    int currentResultsCount() const;
    db::QueryPtr resultAt(int queryIndex) const;

    db::ulonglong rowsFound() const;
    db::ulonglong rowsAffected() const;
    db::ulonglong warningsCount() const;

    int queryTotalCount() const;
    int queryFailedCount() const;

    std::chrono::milliseconds execDuration() const;
    std::chrono::milliseconds networkDuration() const;

    Q_SIGNAL void queryFinished(int queryIndex, int totalCount);

private:
    db::SQLBatch _queries;
    db::Connection * _connection;
    db::user_query::BatchExecutor _executor;
};


} // namespace threads
} // namespace meow

#endif // MEOW_THREADS_QUERY_TASK_H
