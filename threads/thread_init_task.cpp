#include "thread_init_task.h"
#include "db/db_thread_initializer.h"
#include "db/connection.h"

namespace meow {
namespace threads {

DBThreadInitDeinitTask::DBThreadInitDeinitTask(db::Connection * connection)
    : ThreadTask(TaskType::InitDBThread)
    , _initializer(connection->createThreadInitializer())
{

}

void DBThreadInitDeinitTask::run()
{
    if (!_initializer->isInitialized()) {
        _initializer->init();
    } else {
        _initializer->deinit();
    }
    emit finished();
}

bool DBThreadInitDeinitTask::isFailed() const
{
    return false;
}

} // namespace threads
} // namespace meow

