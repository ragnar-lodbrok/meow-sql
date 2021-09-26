#include "db_thread.h"
#include "query_task.h"
#include <QTimer>

namespace meow {
namespace threads {

DbThread::DbThread(db::Connection * connection)
    : _connection(connection)
{

}

std::shared_ptr<QueryTask> DbThread::createQueryTask(
        const db::SQLBatch & queries)
{
    return std::make_shared<QueryTask>(queries, _connection);
}

void DbThread::postTask(const std::shared_ptr<QueryTask> & task)
{
    // TODO: real thread with queue, now emulate async for test
    //QTimer::singleShot(3000, [=](){ task->run(); });
    task->run();
}

} // namespace threads
} // namespace meow
