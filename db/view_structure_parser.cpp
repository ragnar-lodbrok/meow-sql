#include "view_structure_parser.h"
#include "connection.h"
#include "entity/view_entity.h"
#include "helpers/logger.h"
#include "db/query.h"
#include <QDebug>

namespace meow {
namespace db {

ViewStructureParser::ViewStructureParser(Connection * connection)
    :_connection(connection)
{

    _regexp.setPatternOptions(
        QRegularExpression::CaseInsensitiveOption
      | QRegularExpression::InvertedGreedinessOption
      | QRegularExpression::DotMatchesEverythingOption); // AS is multiline in PG

    QString quote = QString(_connection->getIdentQuote()); // TODO: escape

    QString pattern
            = R"(CREATE\s+(OR\s+REPLACE\s+)?)"; // CREATE\s+(OR\s+REPLACE\s+)?
    pattern +=
            R"((ALGORITHM\s*=\s*(\w*)\s*)?)"; // (ALGORITHM\s*=\s*(\w*)\s*)?

    pattern +=
            R"((DEFINER\s*=\s*(\S+|)"; // (DEFINER\s*=\s*(\S+|
    pattern += quote + "[^@" + quote + "]+"
            + quote + "@" + quote
            + "[^" + quote + "]+" + quote
            + R"()\s+)?)"; // )\s+)?

    pattern += R"((SQL\s+SECURITY\s+(\S+)\s+)?)"; // (SQL\s+SECURITY\s+(\S+)\s+)?

    pattern += R"(VIEW\s+[^\(]+\s+)"; // VIEW\s+[^\(]+\s+

    pattern += R"((\([^\)]+\)\s+)?)"; // (\([^\)]+\)\s+)? -- column list

    pattern += R"(AS\s+(.+)(\s+WITH\s+(\w+\s+)?CHECK\s+OPTION\s*)?$)";

    _regexp.setPattern(pattern);

}

void ViewStructureParser::run(ViewEntity * view)
{

    ViewStructure * structure = view->structure();

    structure->clear();

    QString createSQL = view->createCode();

    QRegularExpressionMatch match = _regexp.match(createSQL);

    if (!match.hasMatch()) {
        meowLogCC(Log::Category::Error, _connection)
            << "Unable to parse view " << view->name();
        return;
    }

    if (_connection->connectionParams()->serverType() == ServerType::MySQL) {
        structure->algorithm = match.captured(3).toUpper();
        if (structure->algorithm.isEmpty()) {
            structure->algorithm = "UNDEFINED";
        }
        structure->definer = _connection->dequoteIdentifier(
                    match.captured(5), '@');
        structure->sqlSecurity = match.captured(7).toUpper();
        if (structure->sqlSecurity.isEmpty()) {
            structure->sqlSecurity = "DEFINER";
        }
        structure->checkOption = match.captured(11).toUpper();
    }

    structure->setSelectStatement(match.captured(9));

    getColumns(view);
}

void ViewStructureParser::getColumns(ViewEntity * view)
{
    Connection * connection = view->connection();

    QStringList infoSchemaObjects = connection->informationSchemaObjects();

    // Find "columns" table in information_schema in right case
    QString columnsObjectName;
    for (const QString & object : infoSchemaObjects) {
        if (object.compare("columns", Qt::CaseInsensitive) == 0) {
            columnsObjectName = object;
            break;
        }
    }
    if (columnsObjectName.isEmpty()) {
        return;
    }

    QString schemaClause = tableSchemaColumnName() + '='
            + connection->escapeString(meow::db::databaseName(view));

    QString columnsQuery = "SELECT * FROM "
        + connection->quoteIdentifier(
                connection->informationSchemaDatabaseName())
        + '.' + connection->quoteIdentifier(columnsObjectName)
        + " WHERE " + schemaClause + " AND TABLE_NAME="
        + connection->escapeString(view->name())
        + " ORDER BY ORDINAL_POSITION";

    QueryPtr queryResults = connection->getResults(columnsQuery);

    ITableStructureParser * tableParser = connection->tableStructureParser();

    while (!queryResults->isEof()) {

        TableColumn * column = new TableColumn;

        column->setName(queryResults->curRowColumn("COLUMN_NAME"));

        const QString typeColumn
                = queryResults->columnExists("COLUMN_TYPE")
                ? "COLUMN_TYPE" : "DATA_TYPE";

        QString typeValue = queryResults->curRowColumn(typeColumn);

        column->setDataType(tableParser->extractDataTypeByName(
            typeValue
        ));

        column->setLengthSet(tableParser->extractLengthSet(typeValue));

        // TODO: other columns properties I don't need now

        view->structure()->appendColumn(column);

        queryResults->seekNext();
    }
}

QString ViewStructureParser::tableSchemaColumnName() const
{
    // TODO: as virtual method for each server type
    switch (_connection->connectionParams()->serverType()) {

    case ServerType::MySQL:
        return "TABLE_SCHEMA";

    case ServerType::PostgreSQL:
        return "table_schema";

    case ServerType::SQLite:
        return "TABLE_SCHEMA";

    default:
        return "TABLE_SCHEMA";

    }
}

} // namespace db
} // namespace meow
