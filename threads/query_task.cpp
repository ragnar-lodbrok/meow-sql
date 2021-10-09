#include "query_task.h"
#include "db/connection.h"
#include "db/db_thread_initializer.h"

// test
#include <QThread>
#include <QDebug>
#include "threads/helpers.h"
// test

namespace meow {
namespace threads {

ThreadTask::ThreadTask(TaskType type)
    : QObject(nullptr)
    , _type(type)
{

}

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
    // test
    //QThread::sleep(1);
    //qDebug() << "QueryTask::run() main thread? "
    //         << threads::isCurrentThreadMain();
    // test
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

DbThreadInitTask::DbThreadInitTask(db::Connection * connection)
    : ThreadTask(TaskType::InitDBThread)
    , _initializer(connection->createThreadInitializer())
{

}

void DbThreadInitTask::run()
{
    if (!_initializer->isInitialized()) {
        _initializer->init();
    } else {
        _initializer->deinit();
    }
    emit finished();
}

bool DbThreadInitTask::isFailed() const
{
    return false;
}

} // namespace threads
} // namespace meow
