#include "mysql_database_editor.h"
#include "db/connection.h"

namespace meow {
namespace db {

MySQLDataBaseEditor::MySQLDataBaseEditor(Connection * connection)
    :DataBaseEditor(connection)
{

}

void MySQLDataBaseEditor::create(const QString & name,
                            const QString & collation)
{
    QString SQL = QString("CREATE DATABASE  %1")
            .arg(_connection->quoteIdentifier(name));

    if (collation.isEmpty() == false) {
        SQL += QString("/*!40100 COLLATE %1 */")
                .arg(_connection->escapeString(collation));
    }

    _connection->query(SQL);
}

} // namespace db
} // namespace meow
