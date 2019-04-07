#include "pg_entities_fetcher.h"
#include "db/pg/pg_connection.h"
#include "db/query.h"
#include "db/entity/table_entity.h"
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
    // TODO
    // Empty impl added to avoid crash on expanding session's entities

    fetchTablesViews(dbName, toList);
}

void PGEntitiesFetcher::fetchTablesViews(const QString & dbName,
                                         EntityListForDataBase * toList)
{
    Q_UNUSED(toList);

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

    QString SQL = "SELECT *, " // TODO: don't *
    + sizeSelect + " AS data_length, "
    + "pg_relation_size(" + schemaTable + ")::bigint AS index_length, "
    + "c.reltuples, obj_description(c.oid) AS comment "
    + "FROM " + qu("information_schema") + "." + qu("tables") + " AS t "
    + "LEFT JOIN " + qu("pg_namespace") + " n ON t.table_schema = n.nspname "
    + "LEFT JOIN " + qu("pg_class") + " c ON n.oid = c.relnamespace AND "
    + "c.relname=t.table_name "
    + "WHERE t." + qu("table_schema") + "=" + _connection->escapeString(dbName);
    // TODO: order by ?

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

    while (resPtr->isEof() == false) {

        QString name = resPtr->curRowColumn(indexOfName);

        meowLogDebugC(_connection) << name;

        resPtr->seekNext();
    }
}

QString PGEntitiesFetcher::qu(const char * identifier) const
{
    return _connection->quoteIdentifier(identifier);
}

} // namespace db
} // namespace meow
