#include "trigger_editor.h"
#include "db/entity/trigger_entity.h"
#include "db/connection.h"
#include <QDebug>

namespace meow {
namespace db {


TriggerEditor::TriggerEditor(Connection * connection)
    : _connection(connection)
{

}


bool TriggerEditor::edit(TriggerEntity * trigger, TriggerEntity * newData)
{
    // ALTER trigger is unavailable, so we drop old one, try to create new,
    // and if creating new fails - create back old.

    drop(trigger);

    QString createSQL = getCreateSQL(newData);

    try {
        _connection->query(createSQL);
    } catch (db::Exception & ex) {
        insert(trigger);
        throw;
    }

    return true;
}

bool TriggerEditor::insert(TriggerEntity * trigger)
{
    QString createSQL = getCreateSQL(trigger);

    _connection->query(createSQL);

    return true;
}

bool TriggerEditor::drop(TriggerEntity * trigger)
{
    QString dropSQL = QString("DROP TRIGGER IF EXISTS %1").arg(
                db::quotedName(trigger));
    _connection->query(dropSQL);

    return true;
}

QString TriggerEditor::getCreateSQL(TriggerEntity * trigger)
{
    QString SQL = "CREATE ";

    bool isMySQL
        = _connection->connectionParams()->serverType() == ServerType::MySQL;

    TriggerStructure * structure = trigger->structure();

    if (isMySQL && !structure->definer().isEmpty()) {
        SQL += "DEFINER = "
            + _connection->quoteIdentifier(
                    structure->definer(), true, '@') + " ";
    }

    SQL += "TRIGGER " + _connection->quoteIdentifier(trigger->name()) + " ";

    SQL += structure->actionTime() + " " + structure->event() + " ";

    SQL += "ON " + _connection->quoteIdentifier(structure->tableName()) + " ";

    SQL += "FOR EACH ROW " + structure->statement();

    return SQL;
}

} // namespace db
} // namespace meow
