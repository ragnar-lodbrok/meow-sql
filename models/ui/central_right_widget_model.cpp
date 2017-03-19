#include "central_right_widget_model.h"
#include <QObject> // tr()

namespace meow {
namespace models {
namespace ui {

CentralRightWidgetModel::CentralRightWidgetModel()
    :_currentEntity(nullptr)
{

}

bool CentralRightWidgetModel::setCurrentEntity(db::Entity * currentEntity)
{
    bool changed = _currentEntity != currentEntity;
    _currentEntity = currentEntity;
    return changed;
}

QString CentralRightWidgetModel::titleForHostTab()
{
    if (_currentEntity) {

        QString host = _currentEntity->connection()->connectionParams()->hostName();

        return QObject::tr("Host") + ": " + host;
    } else {
        return QObject::tr("Host");
    }
}

} // namespace ui
} // namespace models
} // namespace meow
