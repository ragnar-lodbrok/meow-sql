#include "central_right_host_widget_model.h"
#include <QObject> // tr()

namespace meow {
namespace models {
namespace ui {

CentralRightHostWidgetModel::CentralRightHostWidgetModel()
    :_curEntity(nullptr)
{

}

bool CentralRightHostWidgetModel::setCurrentEntity(meow::db::SessionEntity * curEntity)
{
    bool changed = _curEntity != curEntity;
    _curEntity = curEntity;
    return changed;
}

QString CentralRightHostWidgetModel::titleForDatabasesTab() const
{
    if (_curEntity) {
        return QObject::tr("Databases") + " (" + QString::number(_curEntity->childCount()) + ")";
    } else {
        return QObject::tr("Databases");
    }
}

} // namespace ui
} // namespace models
} // namespace meow
