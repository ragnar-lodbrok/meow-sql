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

} // namespace db
} // namespace meow
