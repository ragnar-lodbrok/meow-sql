#ifndef MEOW_THREADS_DB_THREAD_H
#define MEOW_THREADS_DB_THREAD_H

#include <QStringList>
#include <QThread>
#include <memory>
#include <list>

namespace meow {

namespace db {

using SQLBatch = QStringList;
class Connection;

}

namespace threads {

class QueriesTask;
class ThreadTask;

// Intent: executes db tasks for connection
class DbThread : public QObject // TODO: rename to TaskThread, ConnectionThread?
{
    Q_OBJECT
public:
    DbThread(db::Connection * connection);
    virtual ~DbThread() override;
    std::shared_ptr<QueriesTask> createQueriesTask(const db::SQLBatch & queries);
    void postTask(const std::shared_ptr<ThreadTask> & task);
    void quit();
    void wait();
private:

    Q_SIGNAL void runTaskAsync();
    Q_SLOT void onTaskFinished();

    void postThreadInitTask();

    db::Connection * _connection;
    QThread * _thread;
    ThreadTask * _initTask;
    std::list<std::shared_ptr<ThreadTask>> _tasks;
};

} // namespace threads
} // namespace meow


#endif // MEOW_THREADS_DB_THREAD_H
