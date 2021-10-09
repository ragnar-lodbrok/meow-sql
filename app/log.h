#ifndef MEOW_LOG_H
#define MEOW_LOG_H

#include <QString>
#include <QList>
#include <QMutex>
#include <QObject>

namespace meow {

namespace db {
    class Connection;
}


// Intent: Central log entry
class Log : public QObject
{
    Q_OBJECT
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

    Log(QObject * parent = nullptr);

    // Thread-safe
    void message(const QString & msg,
                 Category category = Category::Debug,
                 const db::Connection * connection = nullptr);

    // Thread-safe
    void addSink(ISink * sink);

    // Thread-safe
    void removeSink(ISink * sink);

private:
    Q_SLOT void messageMainThread(
            const QString & msg,
            Category category = Category::Debug,
            const QString & sessionName = QString());

    mutable QMutex _mutex;
    QList<ISink *> _sinks;
};

} // namespace meow

#endif // MEOW_LOG_H
