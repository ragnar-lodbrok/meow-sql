#ifndef DB_TABLE_STRUCTURE_PARSER_H
#define DB_TABLE_STRUCTURE_PARSER_H

#include <QList>
#include <QPair>
#include "db/data_type/data_type.h"

namespace meow {
namespace db {

class TableEntity;
class TableColumn;

class TableStructureParser
{
public:
    TableStructureParser();
    void run(TableEntity * table) const;
private:
    void parseColumns(const QString & createSQL, QList<TableColumn *> & columns) const;
    QString extractId(QString & columnString) const;
    DataTypeIndex extractDataTypeByName(QString & columnString) const;
    void prepareTypes() const;
    QString extractLengthSet(QString & columnString) const;
    bool isStartsFromString(QString & columnString, const QString & needle) const;

    mutable QList<QPair<QString, DataTypeIndex>> _types;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_PARSER_H
