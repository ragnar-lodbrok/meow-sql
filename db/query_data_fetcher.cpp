#include "query_data_fetcher.h"
#include "db/common.h"
#include "db/entity/table_entity.h"
#include "query_data.h"
#include "query_criteria.h"
#include "query.h"

namespace meow {
namespace db {

QueryDataFetcher::QueryDataFetcher(Connection * connection)
    :_connection(connection)
{

}

QList<meow::db::TableColumn *>
QueryDataFetcher::partLoadColumns(TableEntity * table)
{
    QList<meow::db::TableColumn *> partLoadColumns;
    for (meow::db::TableColumn * column : table->structure()->columns()) {

        // TODO: get rid of dataTypeLoadPartially
        if (meow::db::dataTypeLoadPartially(column->dataType()->index) == false) {
            continue;
        }

        if (table->structure()->isColumnPrimaryKey(column->name())) {
            continue;
        }

        int length = column->lengthAsInt();
        if (length > db::DATA_MAX_LOAD_TEXT_LEN || length == 0) {
            partLoadColumns << column;
        }
    }
    return partLoadColumns;
}

void QueryDataFetcher::run(
        QueryCriteria * queryCriteria,
        QueryData * toData)
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

} // namespace db
} // namespace meow
