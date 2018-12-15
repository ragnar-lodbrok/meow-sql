#ifndef MEOW_LOG_H
#define MEOW_LOG_H

#include <QString>
#include <QList>

namespace meow {

namespace db {
    class Connection;
}


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

    void message(const QString & msg,
                 Category category = Category::Debug,
                 const db::Connection * connection = nullptr) const;

    void addSink(ISink * sink);
    void removeSink(ISink * sink);

private:
    QList<ISink *> _sinks;
};

} // namespace meow

#endif // MEOW_LOG_H
