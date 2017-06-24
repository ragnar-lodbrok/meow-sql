#include "central_right_widget_model.h"
#include <QObject> // tr()
#include <QDebug>

namespace meow {
namespace models {
namespace ui {

CentralRightWidgetModel::CentralRightWidgetModel()
    : _prevEntity(nullptr),
      _currentEntity(nullptr)
{

}

bool CentralRightWidgetModel::setCurrentEntity(db::Entity * currentEntity)
{
    _prevEntity = _currentEntity;
    _currentEntity = currentEntity;
    return _prevEntity != _currentEntity;
}

bool CentralRightWidgetModel::connectionChanged() const
{
    if (_prevEntity == _currentEntity) {
        return false;
    }

    if (_prevEntity == nullptr || _currentEntity == nullptr) {
        return true;
    }

    return _currentEntity->connection() != _prevEntity->connection();
}

bool CentralRightWidgetModel::databaseChanged() const
{
    if (_prevEntity == _currentEntity) {
        return false;
    }

    if (_prevEntity == nullptr || _currentEntity == nullptr) {
        return true;
    }

    db::Entity * prevDatabaseEntity =
        db::findParentEntityOfType(_prevEntity, db::Entity::Type::Database);

    db::Entity * curDatabaseEntity =
        db::findParentEntityOfType(_currentEntity, db::Entity::Type::Database);

    return prevDatabaseEntity != curDatabaseEntity;
}

bool CentralRightWidgetModel::hasDatabase() const
{
    if (_currentEntity == nullptr) {
        return false;
    }

    return db::findParentEntityOfType(
        _currentEntity, db::Entity::Type::Database) != nullptr;
}

QString CentralRightWidgetModel::titleForHostTab() const
{
    if (_currentEntity) {

        QString host = _currentEntity->connection()->connectionParams()->hostName();

        return QObject::tr("Host") + ": " + host;
    } else {
        return QObject::tr("Host");
    }
}

QString CentralRightWidgetModel::titleForDatabaseTab() const
{
    if (_currentEntity) {

        QString database = _currentEntity->connection()->database();

        return QObject::tr("Database") + ": " + database;
    }

    return QObject::tr("Database");
}

} // namespace ui
} // namespace models
} // namespace meow
