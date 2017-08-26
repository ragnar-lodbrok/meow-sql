#include "central_right_widget.h"
#include "db/entity/entity.h"
#include "app.h"
#include <QDebug>


namespace meow {
namespace ui {
namespace main_window {

CentralRightWidget::CentralRightWidget(QWidget *parent)
    : QWidget(parent),
      _model(),
      _hostTab(nullptr),
      _databaseTab(nullptr),
      _tableTab(nullptr),
      _dataTab(nullptr),
      _queryTab(nullptr)
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
        if (_model.hasDatabase()) {

            db::DataBaseEntity * dbEntity = static_cast<db::DataBaseEntity *>(
                findParentEntityOfType(entity, db::Entity::Type::Database)
            );

            databaseTab()->setDataBase(dbEntity);

            _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Database,
                                  _model.titleForDatabaseTab());
        } else {
            removeDatabaseTab();
        }
    }

    if (_model.hasEntityTab()) {
        if (entity->type() == db::Entity::Type::Table) {
            tableTab();
        }
    } else {
        removeTableTab();
    }

    if (_model.hasDataTab()) {
        bool loadData = onDataTab();
        dataTab()->setDBEntity(entity, loadData);
        _rootTabs->setTabText(_model.indexForDataTab(),
                              _model.titleForDataTab());
    } else {
        removeDataTab();
    }

    if (entity->type() == db::Entity::Type::Session) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Host);
    } else if (entity->type() == db::Entity::Type::Database) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Database);
    } else if (entity->type() == db::Entity::Type::Table) {

        _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Entity,
                              _model.titleForTableTab());
        if ( !onDataTab() && !onQueryTab() ) {
            _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Entity);
        }
    }

    if (_model.hasQueryTab()) {
        queryTab();
    }
}

bool CentralRightWidget::onDataTab() const
{
    return _rootTabs->currentIndex() == _model.indexForDataTab();
}

bool CentralRightWidget::onQueryTab() const
{
    return _rootTabs->currentIndex() == _model.indexForQueryTab();
}

void CentralRightWidget::createRootTabs()
{
    // http://doc.qt.io/qt-5/qtwidgets-dialogs-tabdialog-example.html
    _rootTabs = new QTabWidget();

    connect(_rootTabs,
            &QTabWidget::currentChanged,
            this,
            &CentralRightWidget::rootTabChanged);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

}

void CentralRightWidget::rootTabChanged(int index)
{
    Q_UNUSED(index);
    if (onDataTab()) {
        dataTab()->loadData();
    }
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

central_right::TableTab * CentralRightWidget::tableTab()
{
    if (!_tableTab) {
        _tableTab = new central_right::TableTab();
        _rootTabs->insertTab(models::ui::CentralRightWidgetTabs::Entity,
                             _tableTab,
                             QIcon(":/icons/table.png"),
                             _model.titleForTableTab());
    }

    return _tableTab;
}

central_right::DataTab * CentralRightWidget::dataTab()
{
    if (!_dataTab) {
        _dataTab = new central_right::DataTab();
        _rootTabs->insertTab(_model.indexForDataTab(),
                             _dataTab,
                             QIcon(":/icons/data.png"),
                             _model.titleForDataTab());
    }

    return _dataTab;
}


central_right::QueryTab * CentralRightWidget::queryTab()
{
    if (!_queryTab) {
        auto conMngr = meow::app()->dbConnectionsManager();
        _queryTab = new central_right::QueryTab(conMngr->userQueryAt(0));
        _rootTabs->insertTab(_model.indexForQueryTab(),
                             _queryTab,
                             QIcon(":/icons/execute.png"),
                             _model.titleForQueryTab());
    }

    return _queryTab;
}

bool CentralRightWidget::removeDatabaseTab()
{
    if (_databaseTab) {
        int tabIndex = _rootTabs->indexOf(_databaseTab);
        if (tabIndex >= 0) {
            _rootTabs->removeTab(tabIndex);
        }
        delete _databaseTab;
        _databaseTab = nullptr;
        return (tabIndex >= 0);
    }
    return false;
}

bool CentralRightWidget::removeTableTab()
{
    if (_tableTab) {
        int tabIndex = _rootTabs->indexOf(_tableTab);
        if (tabIndex >= 0) {
            _rootTabs->removeTab(tabIndex);
        }
        delete _tableTab;
        _tableTab = nullptr;
        return (tabIndex >= 0);
    }
    return false;
}

bool CentralRightWidget::removeDataTab()
{
    if (_dataTab) {
        int tabIndex = _rootTabs->indexOf(_dataTab);
        if (tabIndex >= 0) {
            _rootTabs->removeTab(tabIndex);
        }
        delete _dataTab;
        _dataTab = nullptr;
        return (tabIndex >= 0);
    }
    return false;
}

} // namespace meow
} // namespace ui
} // namespace main_window
