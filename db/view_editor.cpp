#include "view_editor.h"
#include "db/entity/view_entity.h"
#include "db/connection.h"

namespace meow {
namespace db {

ViewEditor::ViewEditor(Connection * connection)
    : _connection(connection)
{

}

bool ViewEditor::edit(ViewEntity * view, ViewEntity * newData)
{
    bool structureChanged = !(*(view->structure()) == *(newData->structure()));

    if (structureChanged) {
        QString alterSQL = getSQL(newData->structure(), view->name(), "ALTER");
        _connection->query(alterSQL);
    }

    bool nameChanged = view->name() != newData->name();

    if (nameChanged) {

        QString renameSQL = QString("RENAME TABLE %1 TO %2")
            .arg(_connection->quoteIdentifier(view->name()))
            .arg(_connection->quoteIdentifier(newData->name()));

        _connection->query(renameSQL);
    }

    return structureChanged || nameChanged;
}

bool ViewEditor::insert(ViewEntity * view)
{
    QString createSQL = getSQL(view->structure(), view->name(), "CREATE");
    _connection->query(createSQL);

    return true;
}

bool ViewEditor::drop(ViewEntity * view)
{
    QString dropSQL = QString("DROP VIEW %1").arg(db::quotedName(view));
    _connection->query(dropSQL);

    return true;
}

QString ViewEditor::getSQL(const ViewStructure * view,
                           const QString & viewName,
                           const QString & operation)
{
    QString SQL = operation + " ";

    bool isMySQL
        = _connection->connectionParams()->serverType() == ServerType::MySQL;

    if (isMySQL && !view->algorithm.isEmpty()) {
        SQL += "ALGORITHM = " + view->algorithm + " ";
    }

    if (isMySQL && !view->definer.isEmpty()) {
        SQL += "DEFINER = "
            + _connection->quoteIdentifier(view->definer, true, '@') + " ";
    }

    if (isMySQL && !view->sqlSecurity.isEmpty()) {
        SQL += "SQL SECURITY " + view->sqlSecurity + " ";
    }

    SQL += "VIEW " + _connection->quoteIdentifier(viewName);
    SQL += " AS " + view->selectStatement();

    if (isMySQL
            && !view->checkOption.isEmpty()
            && view->checkOption != "None") {

        SQL += " WITH " + view->checkOption + " CHECK OPTION";
    }

    return SQL;
}

} // namespace db
} // namespace meow
