#include "table_structure_parser.h"

namespace meow {
namespace db {

TableStructureParser::TableStructureParser()
{
}

void TableStructureParser::run(TableEntity * table)
{

    TableStructure * structure = table->structure();
    QString createSQL = table->createCode();

    parseColumns(createSQL, structure->columns());
}

void TableStructureParser::parseColumns(const QString & createSQL,
                                        QList<TableColumn *> & columns)
{
    columns.clear();
}

} // namespace db
} // namespace meow

