#include "log.h"
#include <QDebug>
#include "db/connection.h"

namespace meow {

Log::ISink::~ISink() {}

void Log::message(const QString & msg,
                  Category category,
                  const db::Connection * connection) const
{
    bool isSQL = category == Log::Category::SQL
            || category == Log::Category::UserSQL;

    if (connection) {
        QString logLabel
            = '[' + connection->connectionParams()->sessionName() + ']';
        if (isSQL) {
            logLabel += " [SQL]";
        }
        qDebug().noquote() << logLabel << msg;
    } else {
        if (isSQL) {
            qDebug().noquote() << "[SQL]" << msg;
        } else {
            qDebug().noquote() << msg;
        }
    }

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

    for (auto & sink : _sinks) {
        sink->onLogMessage(messageFormatted);
    }
}

void Log::addSink(ISink * sink)
{
    // Listening: FOR I AM KING - In Flames
    _sinks.push_back(sink);
}

void Log::removeSink(ISink * sink)
{
    _sinks.removeAll(sink);
}

} // namespace meow
