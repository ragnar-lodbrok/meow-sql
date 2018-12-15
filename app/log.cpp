#include "log.h"
#include <QDebug>
#include "db/connection.h"

namespace meow {

Log::Log()
{

}

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

    Q_UNUSED(category);
    Q_UNUSED(connection);
}

} // namespace meow
