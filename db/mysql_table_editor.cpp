#include "mysql_table_editor.h"
#include "mysql_connection.h"
#include "db/entity/table_entity_comparator.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace db {

MySQLTableEditor::MySQLTableEditor(MySQLConnection * connection)
    :TableEditor(connection)
{

}

bool MySQLTableEditor::run(TableEntity * table, TableEntity * newData)
{
    bool changed = false;

    TableEntityComparator diff;
    diff.setCurrTable(newData);
    diff.setPrevTable(table);

    // TODO: begin transaction ?

    if (diff.nameDiffers()) {
        rename(table, newData->name());
        changed = true;
    }

    // TODO: end transaction ?

    return changed;
}

void MySQLTableEditor::rename(TableEntity * table, const QString & newName)
{
    QString SQL = QString("RENAME TABLE %1 TO %2")
            .arg(db::quotedName(table))
            .arg(_connection->quoteIdentifier(newName));

    _connection->query(SQL);
}

} // namespace db
} // namespace meow
