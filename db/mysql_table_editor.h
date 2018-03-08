#ifndef MYSQL_TABLE_EDITOR_H
#define MYSQL_TABLE_EDITOR_H

#include "table_editor.h"
#include <QString>

namespace meow {
namespace db {

class MySQLConnection;
class TableColumn;

class MySQLTableEditor : public TableEditor
{
public:
    MySQLTableEditor(MySQLConnection * connection);
    virtual bool edit(TableEntity * table, TableEntity * newData) override;
    virtual bool insert(TableEntity * table) override;
    
private:
    void rename(TableEntity * table, const QString & newName);
    QString sqlCode(const TableColumn * column) const;
    QString alterColumnSQL(const QString & oldName,
                           const QString & colSQL) const;
};

} // namespace db
} // namespace meow


#endif // MYSQL_TABLE_EDITOR_H
