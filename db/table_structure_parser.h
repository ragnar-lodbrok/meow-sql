#ifndef DB_TABLE_STRUCTURE_PARSER_H
#define DB_TABLE_STRUCTURE_PARSER_H

#include "entity/table_entity.h"

namespace meow {
namespace db {

class TableEntity;

class TableStructureParser
{
public:
    TableStructureParser();
    void run(TableEntity * table);
private:
    void parseColumns(const QString & createSQL, QList<TableColumn *> & columns);
    QString extractId(QString & columnString);
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_PARSER_H
