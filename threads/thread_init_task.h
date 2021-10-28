#ifndef MEOW_THREADS_DBTHREAD_INIT_DEINIT_TASK_H
#define MEOW_THREADS_DBTHREAD_INIT_DEINIT_TASK_H

#include <memory>
#include "thread_task.h"


namespace meow {

namespace db {
class DbThreadInitializer;
class Connection;
}

namespace threads {

class DBThreadInitDeinitTask : public ThreadTask
{
public:
    DBThreadInitDeinitTask(db::Connection * connection);
    virtual void run() override;
    virtual bool isFailed() const override;
private:
    std::unique_ptr<db::DbThreadInitializer> _initializer;
};

} // namespace threads
} // namespace meow

#endif // MEOW_THREADS_DBTHREAD_INIT_DEINIT_TASK_H
