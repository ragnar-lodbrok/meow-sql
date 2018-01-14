#ifndef MYSQL_TABLE_EDITOR_H
#define MYSQL_TABLE_EDITOR_H

#include "table_editor.h"
#include <QString>

namespace meow {
namespace db {

class MySQLConnection;

class MySQLTableEditor : public TableEditor
{
public:
    MySQLTableEditor(MySQLConnection * connection);
    virtual bool run(TableEntity * table, TableEntity * newData) override;
    
private:
    void rename(TableEntity * table, const QString & newName);    
};

} // namespace db
} // namespace meow


#endif // MYSQL_TABLE_EDITOR_H
