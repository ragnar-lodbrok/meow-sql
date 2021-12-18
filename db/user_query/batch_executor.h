#ifndef DB_USER_QUERY_BATCH_EXECUTOR_H
#define DB_USER_QUERY_BATCH_EXECUTOR_H

#include <QStringList>
#include <QList>
#include <QObject>
#include "db/connection.h"

namespace meow {
namespace db {

namespace user_query {

// Thread-safe executor of queries that reports result on the go
class BatchExecutor : public QObject
{
    Q_OBJECT
public:
    BatchExecutor();
    bool run(Connection * connection, const QStringList & queries);

    db::QueryPtr resultAt(int queryIndex) const;
    const db::Exception & error() const {
        QMutexLocker locker(&_mutex);
        return _error;
    }
    bool failed() const {
        QMutexLocker locker(&_mutex);
        return _failed;
    }
    void setStopOnError(bool stop) {
        _stopOnError = stop;
    }
    bool isStopOnError() const {
        return _stopOnError;
    }
    int currentQueryIndex() const {
        QMutexLocker locker(&_mutex);
        return _currentQueryIndex;
    }
    int queryTotalCount() const {
        QMutexLocker locker(&_mutex);
        return _queryTotalCount;
    }
    int queryFailedCount() const {
        QMutexLocker locker(&_mutex);
        return _queryFailedCount;
    }

    db::ulonglong rowsFound() const;
    db::ulonglong rowsAffected() const;
    db::ulonglong warningsCount() const;

    Q_SIGNAL void beforeQueryExecution(int queryIndex, int totalCount);
    Q_SIGNAL void afterQueryExecution(int queryIndex, int totalCount);

private:
    QList<db::QueryPtr> _results;
    db::Exception _error;
    bool _failed;
    int _currentQueryIndex;
    int _queryTotalCount;
    int _queryFailedCount;
    bool _stopOnError = true;

    mutable QMutex _mutex;
};

} // namespace user_query
} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_BATCH_EXECUTOR_H
