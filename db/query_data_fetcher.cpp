#include "query_data_fetcher.h"
#include "db/common.h"
#include "db/entity/table_entity.h"

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

        if (meow::db::dataTypeLoadPartially(column->dataType()) == false) {
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

} // namespace db
} // namespace meow
