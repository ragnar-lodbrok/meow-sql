#include "thread_task.h"

namespace meow {
namespace threads {

ThreadTask::ThreadTask(TaskType type)
    : QObject(nullptr)
    , _type(type)
{

}

} // namespace threads
} // namespace meow
