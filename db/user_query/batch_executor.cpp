#include "batch_executor.h"
#include "db/query.h"
#include "helpers/logger.h"

namespace meow {
namespace db {
namespace user_query {

BatchExecutor::BatchExecutor()
    : _failed(false)
    , _currentQueryIndex(0)
    , _queryTotalCount(0)
    , _queryFailedCount(0)
{

}

bool BatchExecutor::run(Connection * connection, const QStringList & queries)
{
    {
        QMutexLocker locker(&_mutex);
        _results.clear();
        _error = db::Exception();
        _failed = false;

        _currentQueryIndex = 0;
        _queryTotalCount = queries.size();
        _queryFailedCount = 0;
    }

    bool doBreak = false;

    for (const QString & SQL : queries) {

        db::QueryPtr query = connection->createQuery();
        query->setSQL(SQL);

        {
            QMutexLocker locker(&_mutex);
            _results.push_back(query);
        }

        emit beforeQueryExecution(_currentQueryIndex, _queryTotalCount);

        try {
            query->execute();
        } catch(meow::db::Exception & ex) {
            {
                QMutexLocker locker(&_mutex);
                ++_queryFailedCount;
            }
            if (_stopOnError || (_currentQueryIndex == _queryTotalCount - 1)) {
                // TODO: not sure we should have || cond above
                {
                    QMutexLocker locker(&_mutex);
                    _failed = true;
                    _error = ex;
                }
                doBreak = true;
            }
        }

        // TODO: H:
        //Inc(FQueryTime, FConnection.LastQueryDuration);
        //Inc(FQueryNetTime, FConnection.LastQueryNetworkDuration);
        //Inc(FRowsAffected, FConnection.RowsAffected);
        //Inc(FRowsFound, FConnection.RowsFound);
        //Inc(FWarningCount, FConnection.WarningCount);

        emit afterQueryExecution(_currentQueryIndex, _queryTotalCount);

        if (doBreak) break;

        {
            QMutexLocker locker(&_mutex);
            ++_currentQueryIndex;
        }
    }

    return !_failed;
}

db::QueryPtr BatchExecutor::resultAt(int queryIndex) const
{
    QMutexLocker locker(&_mutex);
    return _results.at(queryIndex);
}

db::ulonglong BatchExecutor::rowsFound() const
{
    int sumRowsFound = 0;
    QMutexLocker locker(&_mutex);
    for (const db::QueryPtr & query : _results) {
        sumRowsFound += query->rowsFound();
    }
    return sumRowsFound;
}

db::ulonglong BatchExecutor::rowsAffected() const
{
    int sumRowsAffected = 0;
    QMutexLocker locker(&_mutex);
    for (const db::QueryPtr & query : _results) {
        sumRowsAffected += query->rowsAffected();
    }
    return sumRowsAffected;
}

db::ulonglong BatchExecutor::warningsCount() const
{
    int sumWarningsCount = 0;
    QMutexLocker locker(&_mutex);
    for (const db::QueryPtr & query : _results) {
        sumWarningsCount += query->warningsCount();
    }
    return sumWarningsCount;
}

std::chrono::milliseconds BatchExecutor::execDuration() const
{
    std::chrono::milliseconds sumDuration = std::chrono::milliseconds(0);
    QMutexLocker locker(&_mutex);
    for (const db::QueryPtr & query : _results) {
        sumDuration += query->execDuration();
    }
    return sumDuration;
}

std::chrono::milliseconds BatchExecutor::networkDuration() const
{
    std::chrono::milliseconds sumDuration = std::chrono::milliseconds(0);
    QMutexLocker locker(&_mutex);
    for (const db::QueryPtr & query : _results) {
        sumDuration += query->networkDuration();
    }
    return sumDuration;
}

} // namespace user_query
} // namespace db
} // namespace meow
