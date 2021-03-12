#include "trigger_form.h"
#include "db/connection.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/entity_list_for_database.h"
#include "app/app.h"

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
    , _hasUnsavedChanges(false)
{

}

void TriggerForm::setTrigger(meow::db::TriggerEntity * trigger)
{
    // TODO: copy only when we start editing

    Q_ASSERT(trigger != nullptr);

    if (trigger->isNew()) {
        _sourceTrigger = nullptr;
        _trigger.reset(trigger); // take ownership
        setDefaultValuesForNew();
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

void TriggerForm::setName(const QString & name)
{
    if (_trigger) {
        _trigger->setName(name);
        setHasUnsavedChanges(true);
    }
}

QString TriggerForm::statement() const
{
    if (!_trigger) return {};

    return _trigger->structure()->statement();
}

void TriggerForm::setStatement(const QString & statement)
{
    if (_trigger) {
        _trigger->structure()->setStatement(statement);
        setHasUnsavedChanges(true);
    }
}

QString TriggerForm::definer() const
{
    if (!_trigger) return {};

    return _trigger->structure()->definer();
}

void TriggerForm::setDefiner(const QString & definer)
{
    if (_trigger) {
        _trigger->structure()->setDefiner(definer);
        setHasUnsavedChanges(true);
    }
}

QStringList TriggerForm::allDefinerOptions(bool loadFromDB) const
{
    if (!_trigger) return {};

    if (loadFromDB) {
        return _trigger->connection()->userManager()->allUserNames();
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

void TriggerForm::setTableName(const QString & tableName)
{
    if (_trigger) {
        _trigger->structure()->setTableName(tableName);
        setHasUnsavedChanges(true);
    }
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

void TriggerForm::setActionTime(const QString & actionTime)
{
    if (_trigger) {
        _trigger->structure()->setActionTime(actionTime);
        setHasUnsavedChanges(true);
    }
}

QStringList TriggerForm::allActionTimeOptions() const
{
    if (!_trigger) return {};

    QStringList options;

    options << "BEFORE";
    options << "AFTER";

    return options;
}

QString TriggerForm::eventName() const
{
    if (!_trigger) return {};

    return _trigger->structure()->event();
}

void TriggerForm::setEventName(const QString & eventName)
{
    if (_trigger) {
        _trigger->structure()->setEvent(eventName);
        setHasUnsavedChanges(true);
    }
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

void TriggerForm::save()
{
    if (_trigger->isNew()) { // insert
        meow::db::TriggerEntity * trigger = _trigger.release();
        try {
            bool inserted = meow::app()->dbConnectionsManager()
                    ->activeSession()->insertEntityToDB(trigger);
            if (!inserted) {
                _trigger.reset(trigger);
            }
        } catch (meow::db::Exception & exc) {
            _trigger.reset(trigger);
            throw;
        }

    } else { // update
        meow::app()->dbConnectionsManager()->activeSession()->editEntityInDB(
            _sourceTrigger, _trigger.get());
    }

    setHasUnsavedChanges(false);
}

void TriggerForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

bool TriggerForm::isEditingSupported() const
{
    if (!_trigger) return false;
    return _trigger->connection()->features()
            ->supportsEditingRoutinesStructure();
}

void TriggerForm::setDefaultValuesForNew()
{
    _trigger->structure()->setStatement("BEGIN\n\nEND");
    _trigger->structure()->setDefiner(
        _trigger->connection()->userManager()->currentUserName());
    _trigger->structure()->setActionTime("BEFORE");
    _trigger->structure()->setEvent("INSERT");
}

} // namespace forms
} // namespace models
} // namespace meow
