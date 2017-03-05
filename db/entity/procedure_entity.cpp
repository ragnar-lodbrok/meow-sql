#include "procedure_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

ProcedureEntity::ProcedureEntity(const QString & name, DataBaseEntity * parent)
    :Entity(parent),
     _name(name)
{

}

QString ProcedureEntity::name() const // override
{
    return _name;
}

QVariant ProcedureEntity::icon() const // override
{
    static const QIcon icon = QIcon(":/icons/stored_procedure.png");

    return icon;
}

} // namespace db
} // namespace meow
