#include "mysql_query_data_fetcher.h"
#include "query_criteria.h"
#include "query_data.h"
#include "query.h"
#include "mysql_connection.h"
#include "entity/table_entity.h"
#include "db/common.h"

namespace meow {
namespace db {

MySQLQueryDataFetcher::MySQLQueryDataFetcher(MySQLConnection * connection)
    :QueryDataFetcher(connection)
{

}

void MySQLQueryDataFetcher::run(
        QueryCriteria * queryCriteria,
        QueryData * toData) // override
{

    QString selectList = queryCriteria->select.join(", ");
    if (selectList.isEmpty()) {
        selectList = "*";
    }
    QString select = selectList
            + " FROM " + queryCriteria->quotedDbAndTableName;

    select = _connection->applyQueryLimit("SELECT", select,
                                          queryCriteria->limit,
                                          queryCriteria->offset);

    Query * query = toData->query();
    if (query == nullptr) {
        QueryPtr newQuery = _connection->createQuery();
        toData->setQueryPtr(newQuery);
        query = toData->query();
    }

    query->setSQL(select);

    query->execute(queryCriteria->offset > 0);
}

QStringList MySQLQueryDataFetcher::selectList(TableEntity * table)
{
    QStringList select;
    select << "*";

    QList<meow::db::TableColumn *> partColumns = partLoadColumns(table);

    if (!partColumns.isEmpty()) {
        select.clear();
        for (meow::db::TableColumn * column : table->structure()->columns()) {
            QString name = table->connection()->quoteIdentifier(column->name());
            if (partColumns.contains(column)) {
                name = QString("LEFT(%1, %2) AS %1")
                    .arg(name)
                    .arg(DATA_MAX_LOAD_TEXT_LEN);
            }
            select << name;
        }
    }

    return select;
}

} // namespace db
} // namespace meow
