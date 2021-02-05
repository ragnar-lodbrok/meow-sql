#include "trigger_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

TriggerEntity::TriggerEntity(const QString & name, DataBaseEntity * parent)
    :EntityInDatabase(parent),
     _name(name)
{

}

QString TriggerEntity::name() const // override
{
    return _name;
}

QVariant TriggerEntity::icon() const // override
{
    static const QIcon icon = QIcon(":/icons/trigger.png");

    return icon;
}

TriggerStructure * TriggerEntity::structure() const
{
    if (!_structure) {
        _structure.reset(
            new TriggerStructure(const_cast<TriggerEntity *>(this))
        );
    }
    return _structure.get();
}

DataBaseEntity * TriggerEntity::database() const
{
    return static_cast<DataBaseEntity *>(parent());
}

TriggerEntity * TriggerEntity::deepCopy() const
{
    TriggerEntity * copy = new TriggerEntity(_name, database());

    copy->copyDataFrom(this);

    return copy;
}

void TriggerEntity::copyDataFrom(const Entity * data)
{
    Q_ASSERT(data->type() == Entity::Type::Trigger);

    const TriggerEntity * trigger = static_cast<const TriggerEntity *>(data);

    EntityInDatabase::copyDataFrom(trigger);

    this->_name = trigger->_name;

    this->_structure.reset();

    if (trigger->_structure != nullptr) {
        this->_structure.reset(trigger->_structure->deepCopy(this));
    }
}

} // namespace db
} // namespace meow
