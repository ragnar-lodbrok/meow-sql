#include "routine_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

RoutineEntity::RoutineEntity(const QString & name,
                             Type type,
                             DataBaseEntity * parent)
    : EntityInDatabase(parent)
    , _name(name)
    , _type(type)
{
    Q_ASSERT(type == Type::Procedure || type == Type::Function);
}

QString RoutineEntity::name() const // override
{
    return _name;
}

QVariant RoutineEntity::icon() const // override
{
    static const QIcon procedureIcon = QIcon(":/icons/stored_procedure.png");
    static const QIcon functionIcon = QIcon(":/icons/stored_function.png");

    return (_type == Type::Procedure) ? procedureIcon : functionIcon;
}

RoutineStructure * RoutineEntity::structure() const
{
    if (!_structure) {
        _structure.reset(
            new RoutineStructure(const_cast<RoutineEntity *>(this))
        );
    }
    return _structure.get();
}

RoutineEntity * RoutineEntity::deepCopy() const
{
    RoutineEntity * copy = new RoutineEntity(
                _name,
                _type,
                static_cast<DataBaseEntity *>(parent()));

    copy->copyDataFrom(this);

    return copy;
}

void RoutineEntity::copyDataFrom(const Entity * data)
{
    Q_ASSERT(data->type() == Entity::Type::Procedure
             || data->type() == Entity::Type::Function);

    const RoutineEntity * routine = static_cast<const RoutineEntity *>(data);

    EntityInDatabase::copyDataFrom(routine);

    this->_name = routine->_name;
    this->_type = routine->_type;

    this->_structure.reset();

    if (routine->_structure != nullptr) {
        this->_structure.reset(routine->_structure->deepCopy(this));
    }
}

} // namespace db
} // namespace meow
