#include "pg_entity_create_code_generator.h"
#include "pg_connection.h"
#include "db/entity/table_entity.h"
#include "db/query.h"
#include "helpers/logger.h"

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
    // TODO: this is incorrect and made to be parsed by TableStructureParser only

    QString SQL = "CREATE TABLE " + _connection->quoteIdentifier(table->name());

    QString columnsSQL = createColumnsSQL(table->name());
    QString keysSQL = createKeysSQL(table->name());

    SQL += " (";

    if (!columnsSQL.isEmpty()) {
        SQL += columnsSQL;
        if (!keysSQL.isEmpty()) {
            SQL += ",";
        }
    }

    if (!keysSQL.isEmpty()) {
        SQL += keysSQL;
    }

    SQL += ")";

    // TODO: complete

    return SQL;
}

QString PGEntityCreateCodeGenerator::createColumnsSQL(const QString & tableName)
{
    QueryPtr columnsQuery = _connection->getResults(
            SQLToSelectColumnsInfo(tableName));

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

        columnsQuery->seekNext();

        if (!columnsQuery->isEof()) {
            SQL += ',';
        }
    }

    return SQL;
}

QString PGEntityCreateCodeGenerator::SQLToSelectColumnsInfo(
        const QString & tableName)
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

QString PGEntityCreateCodeGenerator::SQLToSelectKeysInfo(
        const QString & tableName)
{
    if (_connection->serverVersionInt() < MEOW_SERVER_INT(9, 0, 0)) {
        // TODO
        Q_ASSERT(false);
        meowLogDebug() << __FUNCTION__
                       << " not implemented for version "
                       << _connection->serverVersionInt();
        return QString();
    }

    // TODO: CHECK? FOREIGN KEY?

    QString SQL = QString(
    "WITH ndx_list AS (") +
        " SELECT pg_index.indexrelid, pg_class.oid" +
        " FROM pg_index, pg_class" +
        " WHERE pg_class.relname = " + _connection->escapeString(tableName) +
        " AND pg_class.oid = pg_index.indrelid), " +
    " ndx_cols AS (" +
        " SELECT pg_class.relname, UNNEST(i.indkey) AS col_ndx," +
        " CASE i.indisprimary WHEN true THEN " +
            _connection->escapeString("PRIMARY") +
        " ELSE CASE i.indisunique WHEN true THEN " +
            _connection->escapeString("UNIQUE") +
        " ELSE " + _connection->escapeString("KEY") +
        " END END AS CONSTRAINT_TYPE," +
        " pg_class.oid FROM pg_class" +
        " JOIN pg_index i ON (pg_class.oid = i.indexrelid)" +
        " JOIN ndx_list ON (pg_class.oid = ndx_list.indexrelid)" +
    " )" +
    " SELECT ndx_cols.relname AS CONSTRAINT_NAME, ndx_cols.CONSTRAINT_TYPE," +
    " a.attname AS COLUMN_NAME FROM pg_attribute a" +
    " JOIN ndx_cols ON (a.attnum = ndx_cols.col_ndx)" +
    " JOIN ndx_list ON (ndx_list.oid = a.attrelid AND" +
        " ndx_list.indexrelid = ndx_cols.oid)";

    return SQL;
}

QString PGEntityCreateCodeGenerator::createKeysSQL(const QString & tableName)
{
    QueryPtr keysQuery = _connection->getResults(
            SQLToSelectKeysInfo(tableName));

    QString SQL;

    QString constraintName;
    QStringList columnNames;

    while (!keysQuery->isEof()) {

        QString itConstraintName = keysQuery->curRowColumn("CONSTRAINT_NAME");
        if (itConstraintName != constraintName) {
            if (!constraintName.isEmpty()) {
                SQL += " (" + columnNames.join(',') + "),";
            }
            constraintName = itConstraintName;
            QString constraintType = keysQuery->curRowColumn("CONSTRAINT_TYPE");
            SQL += "\n    " + constraintType;
            if (!constraintType.contains("KEY")) {
                SQL += " KEY";
            }
            columnNames.clear();
        }

        QString columnName = keysQuery->curRowColumn("COLUMN_NAME");
        columnNames.append(_connection->quoteIdentifier(columnName));

        keysQuery->seekNext();
    }

    if (!constraintName.isEmpty()) {
        SQL += " (" + columnNames.join(',') + ')';
    }

    return SQL;
}


} // namespace db
} // namespace meow
