#include "central_right_widget.h"
#include "db/entity/entity.h"
#include "app/app.h"
#include <QDebug>
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"

namespace meow {
namespace ui {
namespace main_window {

CentralRightWidget::CentralRightWidget(QWidget *parent)
    : QWidget(parent),
      _model(),
      _hostTab(nullptr),
      _databaseTab(nullptr),
      _tableTab(nullptr),
      _viewTab(nullptr),
      _dataTab(nullptr),
      _queryTab(nullptr)
{

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::entityEdited,
            this,
            &CentralRightWidget::onEntityEdited);

    createRootTabs();
}

void CentralRightWidget::setActiveDBEntity(db::Entity * entity)
{
    bool wasOnDataTab = onDataTab();
    bool wasOnQueryTab = onQueryTab();

    _model.setCurrentEntity(entity);

    if (entity == nullptr) {
        removeAllRootTabs();
        return;
    }

    using Tabs = models::ui::CentralRightWidgetTabs;

    if (_model.connectionChanged()) {
        db::SessionEntity * sessionEntity = static_cast<db::SessionEntity *>(
            findParentEntityOfType(entity, db::Entity::Type::Session)
        );
        hostTab()->setCurrentEntity(sessionEntity);
        _rootTabs->setTabText((int)Tabs::Host,
                              _model.titleForHostTab());
    }

    if (_model.databaseChanged()) {
        if (_model.hasDatabase()) {

            db::DataBaseEntity * dbEntity = static_cast<db::DataBaseEntity *>(
                findParentEntityOfType(entity, db::Entity::Type::Database)
            );

            databaseTab()->setDataBase(dbEntity);

            _rootTabs->setTabText((int)Tabs::Database,
                                  _model.titleForDatabaseTab());
        } else {
            removeDatabaseTab();
        }
    }

    if (_model.hasEntityTab()) {
        removeEntityTabsExcept(entity->type());
        if (entity->type() == db::Entity::Type::Table) {
            auto tableEntity = static_cast<db::TableEntity *>(entity);
            tableTab()->setTable(tableEntity);
        } else if (entity->type() == db::Entity::Type::View) {
            //auto viewEntity = static_cast<db::ViewEntity *>(entity);
            viewTab();//->setView(viewEntity);
        }
    } else {
        removeTableTab();
        removeViewTab();
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
        _rootTabs->setCurrentIndex((int)Tabs::Host);

    } else if (entity->type() == db::Entity::Type::Database) {
        _rootTabs->setCurrentIndex((int)Tabs::Database);

    } else if (entity->type() == db::Entity::Type::Table
               || entity->type() == db::Entity::Type::View) {

        _rootTabs->setTabText((int)Tabs::Entity,
                              _model.titleForEntityTab());

        if ( !wasOnDataTab && !wasOnQueryTab ) {
            _rootTabs->setCurrentIndex((int)Tabs::Entity);
        }
    }

    if (_model.hasQueryTab()) {
        queryTab();
    }
}

void CentralRightWidget::onBeforeEntityEditing()
{
    meow::db::Entity * entity = _model.currentEntity();
    if (!entity) return;

    using Tabs = models::ui::CentralRightWidgetTabs;

    if (_model.hasEntityTab()) {
        if (entity->type() == db::Entity::Type::Table) {
            tableTab()->onBeforeEntityEditing();
            _rootTabs->setCurrentIndex((int)Tabs::Entity);
        }
    }
}

void CentralRightWidget::onEntityEdited(db::Entity * entity)
{
    if (_model.hasEntityTab()) {
        if (entity->type() == db::Entity::Type::Table) {
            _rootTabs->setTabText(
                (int)models::ui::CentralRightWidgetTabs::Entity,
                _model.titleForTableTab());
        }
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

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

}

void CentralRightWidget::rootTabChanged(int index)
{
    if (index < 0) return;

    try {
        if (onDataTab()) {
            dataTab()->loadData();
        }
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

central_right::HostTab * CentralRightWidget::hostTab()
{
    if (!_hostTab) {
        _hostTab = new central_right::HostTab();
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Host,
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
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Database,
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
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Entity,
                             _tableTab,
                             QIcon(":/icons/table.png"),
                             _model.titleForTableTab());
    }

    return _tableTab;
}

central_right::ViewTab * CentralRightWidget::viewTab()
{
    if (!_viewTab) {
        _viewTab = new central_right::ViewTab();
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Entity,
                             _viewTab,
                             QIcon(":/icons/view.png"),
                             _model.titleForViewTab());
    }
    return _viewTab;
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

void CentralRightWidget::removeAllRootTabs()
{
    removeHostTab();
    removeQueryTabs();
    removeDatabaseTab();
    removeTableTab();
    removeViewTab();
    removeDataTab();
}

bool CentralRightWidget::removeHostTab()
{
    if (removeTab(_hostTab)) {
        _hostTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeQueryTabs()
{
    if (removeTab(_queryTab)) {
        _queryTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeDatabaseTab()
{
    if (removeTab(_databaseTab)) {
        _databaseTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeTableTab()
{
    if (removeTab(_tableTab)) {
        _tableTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeViewTab()
{
    if (removeTab(_viewTab)) {
        _viewTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeDataTab()
{
    if (removeTab(_dataTab)) {
        _dataTab = nullptr;
        return true;
    }
    return false;
}

void CentralRightWidget::removeEntityTabsExcept(db::Entity::Type except)
{
    if (except != db::Entity::Type::Table) {
        removeTableTab();
    }
    if (except != db::Entity::Type::View) {
        removeViewTab();
    }
}

bool CentralRightWidget::removeTab(QWidget * tab)
{
    if (tab) {
        int tabIndex = _rootTabs->indexOf(tab);
        if (tabIndex >= 0) {
            _rootTabs->removeTab(tabIndex);
        }
        delete tab;
        return (tabIndex >= 0);
    }
    return false;
}

} // namespace meow
} // namespace ui
} // namespace main_window
