#include <QDebug>
#include "mysql_query.h"

namespace meow {
namespace db {

MySQLQuery::MySQLQuery(Connection *connection)
    :Query(connection)
{

}

void MySQLQuery::execute(bool addResult /*= false*/, int useRawResult /*= -1*/) // override
{
    qDebug() << "[MySQLQuery] " << "Executing: " << SQL();

    // Execute a query, or just take over one of the last result pointers
    if (useRawResult == -1) {
        connection()->query(this->SQL(), true); //H: FStoreResult seems always true?
        useRawResult = 0;
    }

    MYSQL_RES * lastResult = nullptr;
    if (connection()->lastResultsCount() > useRawResult) {
        lastResult = static_cast<MySQLConnection *>(connection())->lastRawResultAt(useRawResult);
    }

    //connection()->query(this->SQL());
}

MySQLQuery::~MySQLQuery()
{

}

} // namespace db
} // namespace meow

