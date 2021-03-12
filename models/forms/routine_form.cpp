#include "routine_form.h"
#include "db/connection.h"
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace models {
namespace forms {

namespace  {

bool isMySQL(meow::db::Connection * connection)
{
    return connection->connectionParams()->serverType()
            == meow::db::ServerType::MySQL;
}

}



RoutineForm::RoutineForm()
    : _routine(nullptr)
    , _sourceRoutine(nullptr)
    , _hasUnsavedChanges(false)
    , _paramsModel(this)
{
    // Listening: Killer Be Killed - Inner Calm From Outer Storms

    connect(&_paramsModel, &models::db::RoutineParametersModel::modified,
            this, &RoutineForm::onParamsModified);
}

void RoutineForm::setRoutine(meow::db::RoutineEntity * routine)
{
    // TODO: copy only when we start editing

    if (routine->isNew()) {
        _sourceRoutine = nullptr;
        _routine.reset(routine); // take ownership
        setDefaultValuesForNew();
    } else {
        _sourceRoutine = routine; // just hold a ref for update
        _routine.reset(_sourceRoutine->deepCopy()); // and edit copy
    }

    setHasUnsavedChanges(false);
}

void RoutineForm::setName(const QString & name)
{
    if (_routine) {
        _routine->setName(name);
        setHasUnsavedChanges(true);
    }
}

QString RoutineForm::definer() const
{
    if (!_routine) return {};

    return _routine->structure()->definer;
}

QStringList RoutineForm::allDefinerOptions(bool loadFromDB) const
{
    if (!_routine) return {};

    if (loadFromDB) {
        return _routine->connection()->userManager()->allUserNames();
    }

    QStringList options;
    QString currentDefiner = definer();
    if (!currentDefiner.isEmpty()) {
        options << definer();
    }

    return options;
}

void RoutineForm::setDefiner(const QString & definer)
{
    if (_routine) {
        _routine->structure()->definer = definer;
        setHasUnsavedChanges(true);
    }
}

bool RoutineForm::supportsDefiner() const
{
    if (_routine) {
        return isMySQL(_routine->connection());
    }
    return false;
}

void RoutineForm::setComment(const QString & comment)
{
    if (_routine) {
        _routine->structure()->comment = comment;
        setHasUnsavedChanges(true);
    }
}

QMap<meow::db::Entity::Type, QString> RoutineForm::typeNames() const
{
    return {
        { meow::db::Entity::Type::Procedure,
                    tr("Procedure (doesn't return a result)") },
        { meow::db::Entity::Type::Function,
                    tr("Function (returns a result)") }
    };
}

void RoutineForm::setType(const meow::db::Entity::Type type)
{
    if (_routine) {
        _routine->setType(type);
        setHasUnsavedChanges(true);
    }
}

QStringList RoutineForm::returnTypes() const
{
    QStringList types;
    types << tr("UNKNOWN");

    const auto & list = _routine->connection()->dataTypes()->list();
    for (const meow::db::DataTypePtr & type : list) {
        types << type->name;
    }

    QString currentType = returnType();
    if (!currentType.isEmpty() && !types.contains(currentType)) {
        types << currentType;
    }
    return types;
}

bool RoutineForm::supportsReturnType() const
{
    return type() == meow::db::Entity::Type::Function;
}

void RoutineForm::setReturnType(const QString & type)
{
    if (_routine) {
        _routine->structure()->returnType = type;
        setHasUnsavedChanges(true);
    }
}

QString RoutineForm::dataAccess() const
{
    return _routine ? _routine->structure()->dataAccess : QString();
}

QStringList RoutineForm::dataAccessList() const
{
    QStringList list;
    if (isMySQL(_routine->connection())) {
        list << "CONTAINS SQL"
             << "NO SQL"
             << "READS SQL DATA"
             << "MODIFIES SQL DATA";
    }
    return list;
}

void RoutineForm::setDataAccess(const QString & dataAccess)
{
    if (_routine) {
        _routine->structure()->dataAccess = dataAccess;
        setHasUnsavedChanges(true);
    }
}

QStringList RoutineForm::securityOptions() const
{
    if (!_routine) return {};

    QStringList options;

    if (isMySQL(_routine->connection())) {
        options << "DEFINER"; // TODO: move to connection?
        options << "INVOKER";
    }

    return options;
}

bool RoutineForm::supportsSecurity() const
{
    if (_routine) {
        return isMySQL(_routine->connection());
    }
    return false;
}

void RoutineForm::setSecurity(const QString & security)
{
    if (_routine) {
        _routine->structure()->sqlSecurity = security;
        setHasUnsavedChanges(true);
    }
}

void RoutineForm::setDeterministic(bool on)
{
    if (_routine) {
        _routine->structure()->deterministic = on;
        setHasUnsavedChanges(true);
    }
}

void RoutineForm::setBody(const QString & body)
{
    if (_routine) {
        _routine->structure()->body = body;
        setHasUnsavedChanges(true);
    }
}

void RoutineForm::save()
{
    if (_routine->isNew()) { // insert
        // try to transfer ownership - take back on error
        // TODO: why not just use shared_ptr ?
        meow::db::RoutineEntity * routine = _routine.release();
        try {
            bool inserted = meow::app()->dbConnectionsManager()
                    ->activeSession()->insertEntityToDB(routine);
            if (!inserted) {
                _routine.reset(routine);
            }
        } catch (meow::db::Exception & exc) {
            _routine.reset(routine);
            throw;
        }

    } else { // update
        meow::app()->dbConnectionsManager()->activeSession()->editEntityInDB(
            _sourceRoutine, _routine.get());
    }

    setHasUnsavedChanges(false);
}

void RoutineForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

bool RoutineForm::isEditingSupported() const
{
    if (!_routine) return false;
    return _routine->connection()->features()
            ->supportsEditingRoutinesStructure();
}

void RoutineForm::setDefaultValuesForNew()
{
    _routine->structure()->body = "BEGIN\n\nEND";
    _routine->structure()->definer
            = _routine->connection()->userManager()->currentUserName();
    _routine->structure()->dataAccess = "CONTAINS SQL";
    _routine->structure()->sqlSecurity = "DEFINER";
}

} // namespace forms
} // namespace models
} // namespace meow
