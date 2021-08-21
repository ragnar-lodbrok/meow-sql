#ifndef MEOW_LOG_H
#define MEOW_LOG_H

#include <QString>
#include <QList>
#include <QMutex>

namespace meow {

namespace db {
    class Connection;
}


// Intent: Central log entry
class Log
{
public:

    enum class Category
    {
        Info,
        SQL,
        UserSQL,
        Error,
        Debug
    };

    class ISink
    {
    public:
        virtual void onLogMessage(const QString & msg) = 0;
        virtual ~ISink();
    };

    // Thread-safe
    void message(const QString & msg,
                 Category category = Category::Debug,
                 const db::Connection * connection = nullptr) const;

    // Thread-safe
    void addSink(ISink * sink);

    // Thread-safe
    void removeSink(ISink * sink);

private:
    mutable QMutex _mutex;
    QList<ISink *> _sinks;
};

} // namespace meow

#endif // MEOW_LOG_H
