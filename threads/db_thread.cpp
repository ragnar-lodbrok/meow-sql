#include "db_thread.h"
#include "queries_task.h"
#include "helpers.h"
#include "thread_init_task.h"
#include <QTimer>
#include <QDebug>

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

std::shared_ptr<QueriesTask> DbThread::createQueriesTask(
        const db::SQLBatch & queries)
{
    return std::make_shared<QueriesTask>(queries, _connection);
}

void DbThread::postTask(const std::shared_ptr<ThreadTask> &task)
{
    MEOW_ASSERT_MAIN_THREAD

    if (_thread) {
        _tasks.push_back(task); // own task ref to avoid external removal
        task->moveToThread(_thread);
        connect(task.get(), &threads::ThreadTask::finished,
                this, &DbThread::onTaskFinished);
        QMetaObject::invokeMethod(task.get(), "run", Qt::QueuedConnection);
    } else {
        task->run();
    }
}

void DbThread::onTaskFinished()
{
    MEOW_ASSERT_MAIN_THREAD

    ThreadTask * finishedTask = static_cast<ThreadTask *>(sender());

    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        if ((*it).get() == finishedTask) {
            _tasks.erase(it);
            break;
        }
    }
}

void DbThread::postThreadInitTask()
{
    if (!_thread) return;

    if (!_initTask) {
        _initTask = new DBThreadInitDeinitTask(_connection);
        _initTask->moveToThread(_thread);
    }
    QMetaObject::invokeMethod(_initTask, "run", Qt::QueuedConnection);
}

} // namespace threads
} // namespace meow
