#include <QDebug>
#include "mysql_entities_fetcher.h"
#include "db/mysql_connection.h"
#include "db/mysql_query.h"
#include "table_entity.h"

namespace meow {
namespace db {

enum SHOW_TABLE_STATUS_COLUMNS {
    Name,
    Engine,
    Version,
    Row_format,
    Rows,
    Avg_row_length,
    Data_length,
    Max_data_length,
    Index_length,
    Data_free,
    Auto_increment,
    Create_time,
    Update_time,
    Check_time,
    Collation,
    Checksum,
    Create_options,
    Comment,
    COUNT
};

MySQLEntitiesFetcher::MySQLEntitiesFetcher(MySQLConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void MySQLEntitiesFetcher::run(const QString & dbName, EntityListForDataBase * toList) // override
{
    fetchTablesViews(dbName, toList);
}

void MySQLEntitiesFetcher::fetchTablesViews(const QString & dbName,
                                            EntityListForDataBase * toList)
{
    MySQLConnection * connection = static_cast<MySQLConnection *>(_connection);

    bool fullTableStatus = connection->connectionParams()->fullTableStatus();

    QueryPtr queryResults;

    if (fullTableStatus || (QString::compare(dbName, "INFORMATION_SCHEMA", Qt::CaseInsensitive) == 0)) {

        try {
            queryResults = connection->getResults(QString("SHOW TABLE STATUS FROM ") + connection->quoteIdentifier(dbName));
        } catch(meow::db::Exception & ex) {
            qDebug() << "[MySQLConnection] " << "Failed to fetch tables/views: " << ex.message();
            return;
        }
    } else {
        qDebug() << "Not implemented";
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {
        while (resPtr->isEof() == false) {

            bool isView = resPtr->isNull(SHOW_TABLE_STATUS_COLUMNS::Engine)
                       && resPtr->isNull(SHOW_TABLE_STATUS_COLUMNS::Version);

            QString name = resPtr->curRowColumn(SHOW_TABLE_STATUS_COLUMNS::Name);

            if (isView) {

            } else {
                TableEntity * table = new TableEntity(name);
                toList->list()->append(table);
            }

            resPtr->seekNext();
        }
    }
}

} // namespace db
} // namespace meow

