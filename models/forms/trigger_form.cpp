#include "trigger_form.h"
#include "db/connection.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/entity_list_for_database.h"

namespace meow {
namespace models {
namespace forms {

namespace  {

bool isMySQL(db::Connection * connection)
{
    return connection->connectionParams()->serverType()
            == db::ServerType::MySQL;
}

}

TriggerForm::TriggerForm()
    : _trigger(nullptr)
    , _sourceTrigger(nullptr)
{

}

void TriggerForm::setTrigger(meow::db::TriggerEntity * trigger)
{
    // TODO: copy only when we start editing

    Q_ASSERT(trigger != nullptr);

    if (trigger->isNew()) {
        _sourceTrigger = nullptr;
        _trigger.reset(trigger); // take ownership
        //setDefaultValuesForNewView();
    } else {
        _sourceTrigger = trigger; // just hold a ref to trigger for update
        _trigger.reset(_sourceTrigger->deepCopy()); // and edit copy
    }

    setHasUnsavedChanges(false);
}

QString TriggerForm::name() const
{
    if (!_trigger) return {};

    return _trigger->name();
}

QString TriggerForm::statement() const
{
    if (!_trigger) return {};

    return _trigger->structure()->statement();
}

QString TriggerForm::definer() const
{
    if (!_trigger) return {};

    return _trigger->structure()->definer();
}

QStringList TriggerForm::allDefinerOptions(bool loadFromDB) const
{
    if (!_trigger) return {};

    if (loadFromDB) {
        return _trigger->connection()->userManager()->allUsers();
    }

    QStringList options;
    QString currentDefiner = definer();
    if (!currentDefiner.isEmpty()) {
        options << definer();
    }

    return options;
}

bool TriggerForm::supportsDefiner() const
{
    if (_trigger) {
        return isMySQL(_trigger->connection());
    }
    return false;
}

QString TriggerForm::tableName() const
{
    if (!_trigger) return {};

    return _trigger->structure()->tableName();
}

QStringList TriggerForm::allTableNameOptions() const
{
    if (!_trigger) return {};

    QStringList tableNames;

    QList<db::Entity *> * entityList
        = _trigger->connection()->getDbEntities(
                _trigger->database()->name())->list();

    for (const db::Entity * entity : *entityList) {
        if (entity->type() == db::Entity::Type::Table) {
            tableNames << entity->name();
        }
    }

    tableNames.sort();

    return tableNames;
}

QString TriggerForm::actionTime() const
{
    if (!_trigger) return {};

    return _trigger->structure()->actionTime();
}

QStringList TriggerForm::allActionTimeOptions() const
{
    if (!_trigger) return {};

    QStringList options;

    options << "BEFORE";
    options << "AFTER";

    return options;
}

QString TriggerForm::event() const
{
    if (!_trigger) return {};

    return _trigger->structure()->event();
}

QStringList TriggerForm::allEventOptions() const
{
    if (!_trigger) return {};

    QStringList options;

    options << "INSERT";
    options << "UPDATE";
    options << "DELETE";

    return options;
}

void TriggerForm::setHasUnsavedChanges(bool modified)
{
    Q_UNUSED(modified);
    // TODO
}

bool TriggerForm::isEditingSupported() const
{
    // TODO
    return false;
}

} // namespace forms
} // namespace models
} // namespace meow
