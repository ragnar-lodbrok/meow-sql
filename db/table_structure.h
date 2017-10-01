#ifndef DB_TABLE_STRUCTURE_H
#define DB_TABLE_STRUCTURE_H

#include "table_column.h"
#include "table_index.h"
#include "foreign_key.h"

namespace meow {
namespace db {

// Holds table structure items
class TableStructure
{
public:
    TableStructure();
    ~TableStructure();

    QList<TableColumn *> & columns() { return _columns; }
    QList<TableIndex  *> & indicies() { return _indicies; }
    QList<ForeignKey  *> & foreighKeys() { return _foreignKeys; }

private:
    QList<TableColumn *>  _columns;
    QList<TableIndex  *>  _indicies;
    QList<ForeignKey  *>  _foreignKeys;

};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_H
