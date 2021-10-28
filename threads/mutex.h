#ifndef MEOW_THREADS_MUTEX_H
#define MEOW_THREADS_MUTEX_H

#include <QMutex>
#include <memory>

namespace meow {
namespace threads {

// Intent: wraps mutex and provides optional dummy implementation for cases
// when threads are not supported or not required.
class Mutex
{
public:
    Mutex(bool dummy = false, bool recursive = false)
        : _mutex(dummy
                 ? nullptr
                 : new QMutex(
                       recursive ? QMutex::Recursive : QMutex::NonRecursive))
    {

    }

    inline void lock()
    {
        if (_mutex) { _mutex->lock(); }
    }

    inline void unlock()
    {
        if (_mutex) { _mutex->unlock(); }
    }

    inline bool tryLock()
    {
        return _mutex ? _mutex->tryLock() : true;
    }

    ~Mutex()
    {
        delete _mutex;
    }

private:
    QMutex * _mutex; // TODO: use std?
    Q_DISABLE_COPY(Mutex)
};

class MutexLocker
{
public:
    explicit inline MutexLocker(Mutex * mutex)
        : _mutex(mutex)
    {
        _mutex->lock();
    }

    inline ~MutexLocker() { _mutex->unlock(); }

private:
    Mutex * _mutex;
    Q_DISABLE_COPY(MutexLocker)
};

// Intent: unlocks locked mutex
class MutexUnlocker
{
public:
    explicit inline MutexUnlocker(Mutex * mutex)
        : _mutex(mutex)
    {
        // TODO: assert mutex is locked
    }

    inline ~MutexUnlocker() { _mutex->unlock(); }

private:
    Mutex * _mutex;
    Q_DISABLE_COPY(MutexUnlocker)
};

} // namespace threads
} // namespace meow

#endif // MEOW_THREADS_MUTEX_H
