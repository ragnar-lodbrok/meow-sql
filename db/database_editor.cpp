#include "database_editor.h"
#include "connection.h"

namespace meow {
namespace db {

DataBaseEditor::DataBaseEditor(Connection * connection) :
    _connection(connection)
{

}

DataBaseEditor::~DataBaseEditor() {}

bool DataBaseEditor::drop(const QString & databaseName)
{
    const QString SQL = QString("DROP DATABASE %1")
            .arg(_connection->quoteIdentifier(databaseName));

    _connection->query(SQL);

    return true;
}

} // namespace db
} // namespace meow
