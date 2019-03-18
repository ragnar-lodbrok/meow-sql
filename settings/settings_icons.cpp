#include "settings_icons.h"
#include "db/table_index.h"
#include "db/connection_parameters.h"

namespace meow {
namespace settings {

Icons::Icons()
{
    setupIcons(); // TODO: lazy loading?
}

void Icons::setupIcons()
{
    _iconsTableIndexClass[db::TableIndexClass::PrimaryKey] =  QIcon(":/icons/key_primary.png");
    _iconsTableIndexClass[db::TableIndexClass::Key] =         QIcon(":/icons/key_index.png");
    _iconsTableIndexClass[db::TableIndexClass::Unique] =      QIcon(":/icons/key_unique.png");
    _iconsTableIndexClass[db::TableIndexClass::FullText] =    QIcon(":/icons/key_primary.png");
    _iconsTableIndexClass[db::TableIndexClass::Spatial] =     QIcon(":/icons/key_fulltext.png");


    _iconsServerType[db::ServerType::MySQL] = QIcon(":/icons/server-mysql.png");
    _iconsServerType[db::ServerType::PostgreSQL] = QIcon(":/icons/server-postgresql.png");
}

const QIcon Icons::iconForTableIndexClass(db::TableIndexClass indexClass) const
{
    return _iconsTableIndexClass.value(indexClass);
}

const QIcon Icons::connection(db::ServerType serverType) const
{
    return _iconsServerType.value(serverType);
}

} // namespace meow
} // namespace settings
