#ifndef MEOW_THREADS_QUERY_TASK_H
#define MEOW_THREADS_QUERY_TASK_H

#include <QObject>
#include <QStringList>
#include "db/user_query/batch_executor.h"
#include "db/query.h"

namespace meow {

namespace db {
using SQLBatch = QStringList;
class Connection;
}

namespace db {
class DbThreadInitializer;
}

namespace threads {

enum class TaskType
{
    Query,
    InitDBThread
};

class ThreadTask : public QObject
{
    Q_OBJECT
public:
    ThreadTask(TaskType type);
    virtual ~ThreadTask() {}
    Q_SLOT virtual void run() = 0;
    virtual bool isFailed() const = 0;

    TaskType type() const { return _type; }

    Q_SIGNAL void finished();
    Q_SIGNAL void failed();

private:
    TaskType _type;
};

class QueryTask : public ThreadTask
{
public:
    QueryTask(const db::SQLBatch & queries, db::Connection * connection);
    ~QueryTask() override;
    void run() override;
    bool isFailed() const override;
    QString errorMessage() const;
    const QList<db::QueryPtr> results() const;
private:
    db::SQLBatch _queries;
    db::Connection * _connection;
    db::user_query::BatchExecutor _executor;
};


// TODO: rename to DBThreadInitDeinitTask
class DbThreadInitTask : public ThreadTask
{
public:
    DbThreadInitTask(db::Connection * connection);
    virtual void run() override;
    virtual bool isFailed() const override;
private:
    std::unique_ptr<db::DbThreadInitializer> _initializer;
};


} // namespace threads
} // namespace meow

#endif // MEOW_THREADS_QUERY_TASK_H
