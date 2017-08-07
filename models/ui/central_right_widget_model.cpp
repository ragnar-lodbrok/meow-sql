#include "central_right_widget_model.h"
#include <QObject> // tr()
#include <QDebug>

namespace meow {
namespace models {
namespace ui {

CentralRightWidgetModel::CentralRightWidgetModel()
    : _entityHolder()
{

}

bool CentralRightWidgetModel::setCurrentEntity(db::Entity * currentEntity)
{
    return _entityHolder.setCurrentEntity(currentEntity);
}

bool CentralRightWidgetModel::connectionChanged() const
{
    return _entityHolder.connectionChanged();
}

bool CentralRightWidgetModel::databaseChanged() const
{
    return _entityHolder.databaseChanged();
}

bool CentralRightWidgetModel::hasDatabase() const
{
    return _entityHolder.hasDatabase();
}

bool CentralRightWidgetModel::hasDataTab() const
{
    if (_entityHolder.currentEntity() == nullptr) {
        return false;
    }

    return _entityHolder.currentEntity()->type() == db::Entity::Type::Table ||
           _entityHolder.currentEntity()->type() == db::Entity::Type::View;
}

bool CentralRightWidgetModel::hasEntityTab() const
{
    if (_entityHolder.currentEntity() == nullptr) {
        return false;
    }
    // TODO: other types
    return _entityHolder.currentEntity()->type() == db::Entity::Type::Table;
}

QString CentralRightWidgetModel::titleForHostTab() const
{
    if (_entityHolder.currentEntity()) {

        QString host = _entityHolder.currentEntity()
                ->connection()->connectionParams()->hostName();

        return QObject::tr("Host") + ": " + host;
    } else {
        return QObject::tr("Host");
    }
}

QString CentralRightWidgetModel::titleForDatabaseTab() const
{
    if (_entityHolder.currentEntity()) {

        QString database = _entityHolder.currentEntity()
                ->connection()->database();

        return QObject::tr("Database") + ": " + database;
    }

    return QObject::tr("Database");
}


QString CentralRightWidgetModel::titleForTableTab() const
{
    if (_entityHolder.currentEntity()
        && _entityHolder.currentEntity()->type() == meow::db::Entity::Type::Table) {
        return QObject::tr("Table") + ": " + _entityHolder.currentEntity()->name();
    }

    return QObject::tr("Table");
}

QString CentralRightWidgetModel::titleForDataTab() const
{
    return QObject::tr("Data");
}

int CentralRightWidgetModel::indexForQueryTab() const
{
    if (hasDataTab()) {
        return CentralRightWidgetTabs::Data + 1;
    } else if (hasEntityTab()) {
        return CentralRightWidgetTabs::Entity + 1;
    } else if (hasDatabase()) {
        return CentralRightWidgetTabs::Database + 1;
    }

    return CentralRightWidgetTabs::Host + 1;
}

} // namespace ui
} // namespace models
} // namespace meow
