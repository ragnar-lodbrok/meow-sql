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

void DataBaseEditor::create(const QString & name,
                            const QString & collation)
{
    Q_UNUSED(collation);

    QString SQL = QString("CREATE DATABASE %1")
            .arg(_connection->quoteIdentifier(name));

    _connection->query(SQL);
}



} // namespace db
} // namespace meow
