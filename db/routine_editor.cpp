#include "routine_editor.h"
#include "db/connection.h"
#include "db/entity/routine_entity.h"
#include <QDebug>

namespace meow {
namespace db {

RoutineEditor::RoutineEditor(Connection * connection)
    : _connection(connection)
{

}

bool RoutineEditor::edit(RoutineEntity * routine, RoutineEntity * newData)
{
    // There is no way to ALTER parameters or the name of it.
    // Create a temp routine, check for syntax errors,
    // then drop the old routine and create it.

    bool isOldProcedure = (routine->type() == db::Entity::Type::Procedure);
    bool isNewProcedure = (newData->type() == db::Entity::Type::Procedure);

    QString oldRoutineSQLType = isOldProcedure ? "PROCEDURE" : "FUNCTION";
    QString newRoutineSQLType = isNewProcedure ? "PROCEDURE" : "FUNCTION";

    QString allRoutinesSQL = QString(
        "SELECT ROUTINE_NAME FROM `information_schema`.`ROUTINES` "
        "WHERE ROUTINE_SCHEMA = %1 "
        "AND ROUTINE_TYPE = %2")
            .arg(_connection->escapeString(_connection->database()))
            .arg(_connection->escapeString(newRoutineSQLType));

    QStringList allRoutineNames = _connection->getColumn(allRoutinesSQL);

    bool nameChanged = routine->name() != newData->name();
    bool typeChanged = routine->type() != newData->type();

    bool targetExists = (nameChanged || typeChanged)
            && allRoutineNames.contains(newData->name());

    if (targetExists) {
        throw db::Exception(
            QObject::tr("Routine `%1` already exists.").arg(newData->name()));
        return false;
    }

    QString tempName;
    for (int i = 1;;++i) {
        tempName = QString("meowsql_temproutine_%1").arg(i);
        if (!allRoutineNames.contains(tempName)) {
            break;
        }
    }

    QString createTempSQL = createSQL(newData, tempName);
    _connection->query(createTempSQL);

    // Drop temporary routine, used for syntax checking
    QString dropTempSQL = QString("DROP %1 IF EXISTS %2")
            .arg(newRoutineSQLType)
            .arg(_connection->quoteIdentifier(tempName));
    _connection->query(dropTempSQL);

    // Drop edited routine
    QString dropEditedSQL = QString("DROP %1 IF EXISTS %2")
            .arg(oldRoutineSQLType)
            .arg(_connection->quoteIdentifier(routine->name()));
    _connection->query(dropEditedSQL);

    /*if (targetExists) { // HeidSQL logic
        // Drop target routine for overwrite
        QString dropTargetSQL = QString("DROP %1 IF EXISTS %2")
                .arg(newRoutineSQLType)
                .arg(_connection->quoteIdentifier(newData->name()));
        _connection->query(dropTargetSQL);
    }*/

    QString createSQL = this->createSQL(newData, newData->name());
    _connection->query(createSQL);

    return true;
}

bool RoutineEditor::insert(RoutineEntity * routine)
{
    QString SQL = createSQL(routine, routine->name());
    _connection->query(SQL);
    return true;
}

bool RoutineEditor::drop(RoutineEntity * routine)
{
    bool isProcedure = (routine->type() == db::Entity::Type::Procedure);

    QString dropSQL = QString("DROP %1 %2")
            .arg(isProcedure ? "PROCEDURE" : "FUNCTION")
            .arg(_connection->quoteIdentifier(routine->name()));

    _connection->query(dropSQL);

    return true;
}

QString RoutineEditor::createSQL(RoutineEntity * routine,
                  const QString & routineName)
{

    RoutineStructure * structure = routine->structure();

    QString SQL = "CREATE ";

    bool isMySQL
        = _connection->connectionParams()->serverType() == ServerType::MySQL;

    if (isMySQL && !structure->definer.isEmpty()) {
        SQL += "DEFINER = "
            + _connection->quoteIdentifier(structure->definer, true, '@') + " ";
    }

    bool isProcedure = (routine->type() == db::Entity::Type::Procedure);

    SQL += isProcedure ? "PROCEDURE" : "FUNCTION";

    SQL += " " + _connection->quoteIdentifier(routineName);

    QStringList params;

    for (const auto & param : structure->params) {
        QString paramStr;
        if (isProcedure) {
            paramStr = param.context + " ";
        }
        paramStr += _connection->quoteIdentifier(param.name);
        paramStr += " ";
        paramStr += param.dataType;

        params << paramStr;
    }

    SQL += "(";
    if (!params.empty()) {
        SQL += "\r\n\t";
        SQL += params.join(",\r\n\t");
    }
    SQL += ")";
    SQL += "\r\n";

    QStringList characteristics;

    if (isProcedure == false) {
        characteristics << "RETURNS " + structure->returnType;
    }
    characteristics << "LANGUAGE SQL";

    if (structure->deterministic) {
        characteristics << "DETERMINISTIC";
    } else {
        characteristics << "NOT DETERMINISTIC";
    }

    characteristics << structure->dataAccess;
    characteristics << "SQL SECURITY " + structure->sqlSecurity;
    characteristics << "COMMENT "
                       + _connection->escapeString(structure->comment);

    SQL += characteristics.join("\r\n");

    SQL += "\r\n";
    SQL += structure->body;

    return SQL;
}

} // namespace db
} // namespace meow
