#include "pg_entities_fetcher.h"
#include "db/pg/pg_connection.h"
#include "db/query.h"
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"
#include "db/entity/function_entity.h"
#include "helpers/logger.h"

namespace meow {
namespace db {


PGEntitiesFetcher::PGEntitiesFetcher(PGConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void PGEntitiesFetcher::run(const QString & dbName,
                            EntityListForDataBase * toList)
{

    fetchTablesViews(dbName, toList);
    fetchStoredFunctions(dbName, toList);
}

void PGEntitiesFetcher::fetchTablesViews(const QString & dbName,
                                         EntityListForDataBase * toList)
{
    QString schemaTable;
    if (_connection->serverVersionInt() >= 70300) {
        schemaTable = "QUOTE_IDENT(t.TABLE_SCHEMA) || "
         + _connection->escapeString(".")
         + " || QUOTE_IDENT(t.TABLE_NAME)";
    } else {
        auto quoteCh = _connection->getIdentQuote();
        QString quoteChEsc = _connection->escapeString(QString(quoteCh));
        schemaTable = quoteChEsc + " || t.TABLE_SCHEMA || "
        + _connection->escapeString(quoteCh + "." + quoteCh)
        + " || t.TABLE_NAME || " + quoteChEsc;
    }

    QString sizeSelect;
    if (_connection->serverVersionInt() >= 90000) {
        sizeSelect = "pg_table_size(" + schemaTable + ")::bigint";
    } else {
        sizeSelect = "NULL";
    }

    QString SQL = "SELECT t.table_name, t.table_type, t.table_schema, "
    + sizeSelect + " AS data_length, "
    + "pg_relation_size(" + schemaTable + ")::bigint AS index_length, "
    + "c.reltuples, obj_description(c.oid) AS comment "
    + "FROM " + qu("information_schema") + "." + qu("tables") + " AS t "
    + "LEFT JOIN " + qu("pg_namespace") + " n ON t.table_schema = n.nspname "
    + "LEFT JOIN " + qu("pg_class") + " c ON n.oid = c.relnamespace AND "
    + "c.relname=t.table_name "
    + "WHERE t." + qu("table_schema") + "=" + _connection->escapeString(dbName)
    + " ORDER BY t.table_name";

    QueryPtr queryResults;

    try {
        queryResults = _connection->getResults(SQL);
    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
                << "Failed to fetch tables/views: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    std::size_t indexOfName = resPtr->indexOfColumn("table_name");
    std::size_t indexOfRows = resPtr->indexOfColumn("reltuples");
    std::size_t indexOfDataLen = resPtr->indexOfColumn("data_length");
    std::size_t indexOfIndexLen  = resPtr->indexOfColumn("index_length");
    std::size_t indexOfTableType = resPtr->indexOfColumn("table_type");
    std::size_t indexOfTableSchema = resPtr->indexOfColumn("table_schema");
    Q_UNUSED(indexOfTableSchema); // TODO

    while (resPtr->isEof() == false) {

        QString name = resPtr->curRowColumn(indexOfName);
        QString type = resPtr->curRowColumn(indexOfTableType);

        bool isView = (type == "VIEW");

        if (isView) {
            ViewEntity * view = new ViewEntity(name);
            toList->list()->append(view);
        } else {
            TableEntity * table = new TableEntity(name);

            table->setRowsCount(
                resPtr->curRowColumn(indexOfRows).toULongLong()
            );

            auto dataLen = resPtr->curRowColumn(indexOfDataLen).toULongLong();
            auto indexLen = resPtr->curRowColumn(indexOfIndexLen).toULongLong();
            table->setDataSize(dataLen + indexLen);

            toList->list()->append(table);
        }

        resPtr->seekNext();
    }
}

void PGEntitiesFetcher::fetchStoredFunctions(const QString & dbName,
                                             EntityListForDataBase * toList)
{
    QueryPtr queryResults;

    const QString pDot = qu("p") + ".";
    const QString nDot = qu("n") + ".";
    const QString cDot = qu("pg_catalog") + ".";

    const QString SQL = "SELECT " + pDot + qu("proname") + ", "
    + pDot + qu("proargtypes")
    + " FROM " + cDot + qu("pg_namespace") + " AS " + qu("n")
    + " JOIN " + cDot + qu("pg_proc")      + " AS " + qu("p")
    + " ON " + pDot + qu("pronamespace") + " = " + nDot + qu("oid")
    + " WHERE " + nDot + qu("nspname")
    + " = " + _connection->escapeString(dbName)
    + " ORDER BY " + pDot + qu("proname");

    try {
        queryResults = _connection->getResults(SQL);
    } catch (meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
                << "Failed to fetch stored functions: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    std::size_t indexOfName     = resPtr->indexOfColumn("proname");
    std::size_t indexOfArgTypes  = resPtr->indexOfColumn("proargtypes");
    Q_UNUSED(indexOfArgTypes); // TODO

    while (resPtr->isEof() == false) {

        QString name = resPtr->curRowColumn(indexOfName);

        FunctionEntity * func = new FunctionEntity(name);

        toList->list()->append(func);

        resPtr->seekNext();
    }

}


QString PGEntitiesFetcher::qu(const char * identifier) const
{
    return _connection->quoteIdentifier(identifier);
}

} // namespace db
} // namespace meow
