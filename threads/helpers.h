#ifndef MEOW_THREADS_HELPERS_H
#define MEOW_THREADS_HELPERS_H

#include <QApplication>
#include <QThread>

namespace meow {
namespace threads {

static bool isCurrentThreadMain()
{
    return QApplication::instance()->thread() == QThread::currentThread();
}

} // namespace threads
} // namespace meow

#define MEOW_ASSERT_MAIN_THREAD Q_ASSERT(meow::threads::isCurrentThreadMain());

#endif // MEOW_THREADS_HELPERS_H
