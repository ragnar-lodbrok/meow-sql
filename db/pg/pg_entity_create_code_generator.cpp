#include "pg_entity_create_code_generator.h"
#include "pg_connection.h"
#include "db/entity/table_entity.h"
#include "db/query.h"
#include <QDebug>

namespace meow {
namespace db {

PGEntityCreateCodeGenerator::PGEntityCreateCodeGenerator(
    PGConnection *connection)
    : _connection(connection)
{

}

QString PGEntityCreateCodeGenerator::run(const Entity * entity)
{
    switch (entity->type()) {

    case Entity::Type::Table: {
        auto table = static_cast<const TableEntity *>(entity);
        return run(table);
    }

    default:
        Q_ASSERT(false);
        return QString();
    }
}

QString PGEntityCreateCodeGenerator::run(const TableEntity * table)
{
    QString SQL = "CREATE TABLE " + _connection->quoteIdentifier(table->name());

    SQL += " (" + createColumnsSQL(table->name()) + ")";

    // TODO: complete

    return SQL;
}

QString PGEntityCreateCodeGenerator::createColumnsSQL(const QString & tableName)
{
    QueryPtr columnsQuery = _connection->getResults(
            selectColumnsSQL(tableName));

    QString SQL;

    std::size_t indexOfCharMaxLen
        = columnsQuery->indexOfColumn("CHARACTER_MAXIMUM_LENGTH");
    std::size_t indexOfColumnDefault
        = columnsQuery->indexOfColumn("COLUMN_DEFAULT");

    while (!columnsQuery->isEof()) {

        QString dataType = columnsQuery->curRowColumn("DATA_TYPE").toUpper();
        QString name = columnsQuery->curRowColumn("COLUMN_NAME");

        SQL += "\n    " + _connection->quoteIdentifier(name) + " " + dataType;

        QString maxLen;
        if (!columnsQuery->isNull(indexOfCharMaxLen)) {
            maxLen = columnsQuery->curRowColumn(indexOfCharMaxLen);
            if (maxLen == "-1") {
                maxLen = "max";
            }
        }
        if (!maxLen.isEmpty()) {
            SQL += '(' + maxLen + ')';
        }

        if (columnsQuery->curRowColumn("IS_NULLABLE") == "NO") {
            SQL += " NOT";
        }
        SQL += " NULL";

        if (columnsQuery->isNull(indexOfColumnDefault)) {
            if (columnsQuery->curRowColumn("IS_NULLABLE") != "NO") {
                SQL += " DEFAULT NULL";
            }
        } else {
            SQL += " DEFAULT " + columnsQuery->curRowColumn(indexOfColumnDefault);
        }

        // TODO: H: column comment

        SQL += ',';
        columnsQuery->seekNext();
    }

    return SQL;
}

QString PGEntityCreateCodeGenerator::selectColumnsSQL(const QString & tableName)
{
    QString SQL = QString("SELECT DISTINCT a.attname AS column_name,") +
    " a.attnum, a.atttypid, FORMAT_TYPE(a.atttypid, a.atttypmod) AS data_type," +
    " CASE a.attnotnull WHEN false THEN " + _connection->escapeString("YES") +
    " ELSE " + _connection->escapeString("NO") + " END AS IS_NULLABLE," +
    " com.description AS column_comment, def.adsrc AS column_default, " +
    " NULL AS character_maximum_length" +
    " FROM pg_attribute AS a" +
    " JOIN pg_class AS pgc ON pgc.oid = a.attrelid" +
    " LEFT JOIN pg_description AS com" +
        " ON (pgc.oid = com.objoid AND a.attnum = com.objsubid)" +
    " LEFT JOIN pg_attrdef AS def" +
        " ON (a.attrelid = def.adrelid AND a.attnum = def.adnum) " +
    " WHERE" +
        " a.attnum > 0 AND pgc.oid = a.attrelid" +
        " AND pg_table_is_visible(pgc.oid)" +
        " AND NOT a.attisdropped" +
        " AND pgc.relname = " + _connection->escapeString(tableName) +
    " ORDER BY a.attnum";

    return SQL;
}


} // namespace db
} // namespace meow
