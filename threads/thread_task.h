#ifndef MEOW_THREADS_THREAD_TASK_H
#define MEOW_THREADS_THREAD_TASK_H

#include <QObject>

namespace meow {
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

} // namespace threads
} // namespace meow

#endif // MEOW_THREADS_THREAD_TASK_H
