#include "central_right_host_widget_model.h"
#include "ui/models/variables_table_model.h"
#include "db/connection.h"
#include "db/connection_features.h"
#include <QObject> // tr()

namespace meow {
namespace ui {
namespace presenters {

CentralRightHostWidgetModel::CentralRightHostWidgetModel()
    :_curEntity(nullptr)
    ,_variablesModel(nullptr)
{

}

bool CentralRightHostWidgetModel::setCurrentEntity(
        meow::db::SessionEntity * curEntity)
{
    bool changed = _curEntity != curEntity;
    _curEntity = curEntity;
    return changed;
}

QString CentralRightHostWidgetModel::titleForDatabasesTab() const
{
    if (_curEntity) {
        return QObject::tr("Databases") + " (" +
                QString::number(_curEntity->childCount()) + ")";
    } else {
        return QObject::tr("Databases");
    }
}

QString CentralRightHostWidgetModel::titleForVariablesTab() const
{
    if (_curEntity && _variablesModel) {
        int count = _variablesModel->rowCount();
        if (count) {
            return QObject::tr("Variables") + " (" +
                    QString::number(count) + ")";
        }
    }

    return QObject::tr("Variables");
}

bool CentralRightHostWidgetModel::showVariablesTab() const
{
    if (_curEntity) {
        return _curEntity->connection()->features()->supportsViewingVariables();
    } else {
        return false;
    }
}

} // namespace presenters
} // namespace ui
} // namespace meow
