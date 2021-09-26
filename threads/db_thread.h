#ifndef MEOW_THREADS_DB_THREAD_H
#define MEOW_THREADS_DB_THREAD_H

#include <QStringList>
#include <memory>

namespace meow {

namespace db {

using SQLBatch = QStringList;
class Connection;

}

namespace threads {

class QueryTask;
class IThreadTask;

// Intent: executes db tasks for connection
class DbThread // TODO: rename to TaskThread, ConnectionThread?
{
public:
    DbThread(db::Connection * connection);
    std::shared_ptr<QueryTask> createQueryTask(const db::SQLBatch & queries);
    void postTask(const std::shared_ptr<QueryTask> & task);
private:
    db::Connection * _connection;
};

} // namespace threads
} // namespace meow


#endif // MEOW_THREADS_DB_THREAD_H
