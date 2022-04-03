#ifndef DB_SQLITE_TABLE_STRUCTURE_PARSER_H
#define DB_SQLITE_TABLE_STRUCTURE_PARSER_H

#include "db/table_structure_parser.h"

namespace meow {
namespace db {

class SQLiteTableStructureParser : public ITableStructureParser
{
public:
    explicit SQLiteTableStructureParser(Connection * connection);
    virtual void run(TableEntity * table) override;
    virtual DataTypePtr extractDataTypeByName(QString & columnString) override;
    virtual QString extractLengthSet(QString & columnString) override;
};

} // namespace db
} // namespace meow


#endif // DB_SQLITE_TABLE_STRUCTURE_PARSER_H
