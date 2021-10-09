#ifndef MEOW_THREADS_DB_THREAD_H
#define MEOW_THREADS_DB_THREAD_H

#include <QStringList>
#include <QThread>
#include <memory>

namespace meow {

namespace db {

using SQLBatch = QStringList;
class Connection;

}

namespace threads {

class QueryTask;
class ThreadTask;

// Intent: executes db tasks for connection
class DbThread : public QObject // TODO: rename to TaskThread, ConnectionThread?
{
    Q_OBJECT
public:
    DbThread(db::Connection * connection);
    virtual ~DbThread() override;
    std::shared_ptr<QueryTask> createQueryTask(const db::SQLBatch & queries);
    void postTask(const std::shared_ptr<ThreadTask> & task);
    void quit();
    void wait();
private:

    Q_SIGNAL void runTaskAsync();

    void postThreadInitTask();

    db::Connection * _connection;
    QThread * _thread;
    ThreadTask * _initTask;
};

} // namespace threads
} // namespace meow


#endif // MEOW_THREADS_DB_THREAD_H
