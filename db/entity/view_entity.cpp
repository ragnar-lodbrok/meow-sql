#include "view_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

ViewEntity::ViewEntity(const QString & viewName, DataBaseEntity * parent)
    :EntityInDatabase(parent),
     _viewName(viewName)
{

}

QString ViewEntity::name() const // override
{
    return _viewName;
}

QVariant ViewEntity::icon() const // override
{
    // TODO: not here
    static const QIcon icon = QIcon(":/icons/view.png");

    return icon;
}

ViewStructure * ViewEntity::structure() const
{
    if (!_structure) {
        _structure.reset(
            new ViewStructure(const_cast<ViewEntity *>(this))
        );
    }
    return _structure.get();
}

} // namespace db
} // namespace meow
