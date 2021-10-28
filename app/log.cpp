#include "log.h"
#include <QDebug>
#include "db/connection.h"
#include "threads/helpers.h"

namespace meow {

Log::ISink::~ISink() {}

Log::Log(QObject * parent) : QObject(parent)
{
    qRegisterMetaType<Category>("Category");
}

void Log::message(
        const QString & msg,
        Category category,
        const db::Connection * connection)
{
    // We expect all sinks want to receive messages in main thread.
    // Otherwise change the logic here.

    QString sessionName;
    if (connection) {
        sessionName = connection->connectionParams()->sessionName();
    }

    if (threads::isCurrentThreadMain()) {
        messageMainThread(msg, category, sessionName);
    } else {
        QMetaObject::invokeMethod(
                    this,
                    "messageMainThread",
                    Qt::QueuedConnection,
                    Q_ARG(QString, msg),
                    Q_ARG(Category, category),
                    Q_ARG(QString, sessionName));
    }
}

void Log::messageMainThread(
        const QString & msg,
        Category category,
        const QString & sessionName)
{
    bool isSQL = category == Log::Category::SQL
            || category == Log::Category::UserSQL;


#ifndef NDEBUG
    if (!sessionName.isEmpty()) {
        QString logLabel
            = '[' + sessionName + ']';
        if (isSQL) {
            logLabel += " [SQL]";
        }
        // Note: qDebug is thread-safe
        qDebug().noquote() << logLabel << msg;
    } else {
        if (isSQL) {
            qDebug().noquote() << "[SQL]" << msg;
        } else {
            qDebug().noquote() << msg;
        }
    }
#endif
    Q_UNUSED(sessionName);

    bool doLog = false;

    switch (category) { // TODO: use settings for loggable categories
    case Category::SQL:
    case Category::UserSQL:
    case Category::Error:
    case Category::Info:
        doLog = true;
        break;
    default:
        break;
    };

    if (!doLog) return;

    QString messageFormatted = msg;

    // TODO: limit len of message

    if (isSQL) {
        if (!messageFormatted.endsWith(';')) {
            messageFormatted += ';'; // TODO: take delim from outside
        }
    } else {
        messageFormatted = "/* " + messageFormatted + " */"; // TODO: escape?
    }

    QMutexLocker locker(&_mutex);

    for (auto & sink : _sinks) {
        sink->onLogMessage(messageFormatted);
    }
}

void Log::addSink(ISink * sink)
{
    // Listening: FOR I AM KING - In Flames
    QMutexLocker locker(&_mutex);
    _sinks.push_back(sink);
}

void Log::removeSink(ISink * sink)
{
    QMutexLocker locker(&_mutex);
    _sinks.removeAll(sink);
}

} // namespace meow
