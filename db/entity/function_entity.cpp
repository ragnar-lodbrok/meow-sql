#include "function_entity.h"

#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

FunctionEntity::FunctionEntity(const QString & name, DataBaseEntity * parent)
    :EntityInDatabase(parent),
     _name(name)
{

}

QString FunctionEntity::name() const // override
{
    return _name;
}

QVariant FunctionEntity::icon() const // override
{
    static const QIcon icon = QIcon(":/icons/stored_function.png");

    return icon;
}

} // namespace db
} // namespace meow
