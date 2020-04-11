#include "sqlite_table_structure_parser.h"
#include "utils/sql_parser/sqlite/sqlite_parser.h"
#include "db/entity/table_entity.h"
#include "db/data_type/sqlite_connection_datatypes.h"
#include <QDebug>

namespace meow {
namespace db {

SQLiteTableStructureParser::SQLiteTableStructureParser(
        Connection * connection)
    :ITableStructureParser(connection)
{
    // Listening: Metallica - Fight Fire With Fire
}


void SQLiteTableStructureParser::run(TableEntity * table)
{

    TableStructure * structure = table->structure();

    structure->clearColumns(); // TODO: add clear method of structure
    structure->removeAllIndicies();
    structure->removeAllIndicies();


    utils::sql_parser::SQLiteParser parser;

    bool success = parser.parseCreateTable(table->createCode().toStdString());

    if (!success) {
        // TODO: log error
        return;
    }

    qDebug() << "\n\ncolumns: ";

    auto types = static_cast<SQLiteConnectionDataTypes *>(
                    _connection->dataTypes());

    using Column = meow::utils::sql_parser::SQLiteColumnPtr;

    const std::vector<Column> & columns = parser._parsedTable->columns();

    for (const Column & parsedCol : columns ) {
        TableColumn * column = new TableColumn();

        column->setName(QString::fromStdString(parsedCol->name()));

        DataTypePtr basicDataType = types->dataTypeByName(
                QString::fromStdString(parsedCol->type()));

        // make copy of data type and change type name to real
        auto colDataType = DataTypePtr(new DataType(*basicDataType));
        colDataType->name = QString::fromStdString(parsedCol->type());

        column->setDataType(colDataType);

        structure->appendColumn(column);

        qDebug() << column->operator QString();
    }
}


} // namespace db
} // namespace meow
