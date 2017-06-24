#include "central_right_widget.h"
#include "db/entity/entity.h"
#include <QDebug>


namespace meow {
namespace ui {
namespace main_window {

CentralRightWidget::CentralRightWidget(QWidget *parent)
    : QWidget(parent),
      _model(),
      _hostTab(nullptr),
      _databaseTab(nullptr)
{
    createRootTabs();
}

void CentralRightWidget::setActiveDBEntity(db::Entity * entity)
{
    _model.setCurrentEntity(entity);

    if (entity == nullptr) {
        return;
    }

    if (_model.connectionChanged()) {
        db::SessionEntity * sessionEntity = static_cast<db::SessionEntity *>(
            findParentEntityOfType(entity, db::Entity::Type::Session)
        );
        hostTab()->setCurrentEntity(sessionEntity);
        _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Host,
                              _model.titleForHostTab());
    }

    if (_model.databaseChanged()) {

        db::DataBaseEntity * dbEntity = static_cast<db::DataBaseEntity *>(
            findParentEntityOfType(entity, db::Entity::Type::Database)
        );

        databaseTab()->setDataBase(dbEntity);

        if (_model.hasDatabase()) {
            _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Database,
                                  _model.titleForDatabaseTab());
        } else {
            //_rootTabs->removeTab(models::ui::CentralRightWidgetTabs::Database);
        }
    }

    if (entity->type() == db::Entity::Type::Session) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Host);
    } else if (entity->type() == db::Entity::Type::Database) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Database);
    }
}

void CentralRightWidget::createRootTabs()
{
    // http://doc.qt.io/qt-5/qtwidgets-dialogs-tabdialog-example.html
    _rootTabs = new QTabWidget();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

}

central_right::HostTab * CentralRightWidget::hostTab()
{
    if (!_hostTab) {
        _hostTab = new central_right::HostTab();
        _rootTabs->insertTab(models::ui::CentralRightWidgetTabs::Host,
                             _hostTab,
                             QIcon(":/icons/host.png"),
                             _model.titleForHostTab());
    }

    return _hostTab;
}

central_right::DatabaseTab * CentralRightWidget::databaseTab()
{
    if (!_databaseTab) {
        _databaseTab = new central_right::DatabaseTab();
        _rootTabs->insertTab(models::ui::CentralRightWidgetTabs::Database,
                             _databaseTab,
                             QIcon(":/icons/database.png"),
                             _model.titleForDatabaseTab());
    }

    return _databaseTab;
}

} // namespace meow
} // namespace ui
} // namespace main_window
