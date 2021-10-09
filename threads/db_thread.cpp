#include "db_thread.h"
#include "query_task.h"
#include <QTimer>

namespace meow {
namespace threads {

DbThread::DbThread(db::Connection * connection)
    : QObject(nullptr)
    , _connection(connection)
    , _thread(
          connection->features()->supportsMultithreading()
          ? new QThread
          : nullptr)
    , _initTask(nullptr)
{

    if (_thread) {
        _thread->start();
        postThreadInitTask();
    }
}

DbThread::~DbThread()
{
    if (_thread) {
        postThreadInitTask();
    }
    delete _initTask;
    quit();
    wait();
    delete _thread;
}

void DbThread::quit()
{
    if (_thread) {
        _thread->quit();
    }
}

void DbThread::wait()
{
    if (_thread) {
        _thread->wait();
    }
}

std::shared_ptr<QueryTask> DbThread::createQueryTask(
        const db::SQLBatch & queries)
{
    return std::make_shared<QueryTask>(queries, _connection);
}

void DbThread::postTask(const std::shared_ptr<ThreadTask> &task)
{
    if (_thread) {
        // TODO: add queue of tasks to own them as someone can delete task?
        task->moveToThread(_thread);
        QMetaObject::invokeMethod(task.get(), "run", Qt::QueuedConnection);
    } else {
        task->run();
    }
}

void DbThread::postThreadInitTask()
{
    if (!_thread) return;

    if (!_initTask) {
        _initTask = new DbThreadInitTask(_connection);
        _initTask->moveToThread(_thread);
    }
    QMetaObject::invokeMethod(_initTask, "run", Qt::QueuedConnection);
}

} // namespace threads
} // namespace meow
