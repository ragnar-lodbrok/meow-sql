#include "routine_form.h"
#include "db/connection.h"

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

}

void RoutineForm::setRoutine(meow::db::RoutineEntity * routine)
{
    // TODO: copy only when we start editing

    if (routine->isNew()) {
        _sourceRoutine = nullptr;
        _routine.reset(routine); // take ownership
        //setDefaultValuesForNewRoutine(); // TODO
    } else {
        _sourceRoutine = routine; // just hold a ref for update
        _routine.reset(_sourceRoutine->deepCopy()); // and edit copy
    }

    _paramsModel.setRoutine(_routine.get());

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
        return _routine->connection()->userManager()->allUsers();
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

QMap<meow::db::Entity::Type, QString> RoutineForm::typeNames() const
{
    return {
        { meow::db::Entity::Type::Procedure,
                    tr("Procedure (doesn't return a result)") },
        { meow::db::Entity::Type::Function,
                    tr("Function (returns a result)") }
    };
}

QStringList RoutineForm::returnTypes() const
{
    QStringList types;
    types << tr("None");
    QString currentType = returnType();
    if (!currentType.isEmpty()) {
        types << currentType;
    }
    return types;
}

bool RoutineForm::supportsReturnType() const
{
    return type() == meow::db::Entity::Type::Function;
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

} // namespace forms
} // namespace models
} // namespace meow
