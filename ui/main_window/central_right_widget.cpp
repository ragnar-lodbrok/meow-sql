#include "central_right_widget.h"
#include "db/entity/entity.h"
#include "app/app.h"
#include <QDebug>
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"
#include "db/entity/routine_entity.h"
#include "db/entity/trigger_entity.h"

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
      _routineTab(nullptr),
      _triggerTab(nullptr),
      _dataTab(nullptr),
      _queryTab(nullptr)
{

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::entityEdited,
            this,
            &CentralRightWidget::onEntityEdited);

    createWidgets();

    connect(meow::app()->settings()->geometrySettings(),
            &settings::Geometry::showFilterPanelChanged,
            [=](bool enabledInSettings){
                bool show = showGlobalFilterPanel();
                _filterWidget->setVisible(show);
                if (show) {
                    _filterWidget->setFocus();
                }
                if (!enabledInSettings) {
                    _filterWidget->reset();
                }
            }
    );
}

void CentralRightWidget::setActiveDBEntity(db::Entity * entity)
{
    _filterWidget->reset();

    bool wasOnDataTab = entity ? onDataTab() : false;
    bool wasOnQueryTab = entity ? onQueryTab() : false;

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

    bool isTable = false;
    bool isView = false;
    bool isFunction = false;
    bool isProcedure = false;
    bool isRoutine = false;
    bool isTrigger = false;

    if (_model.hasEntityTab()) {

        isTable = entity->type() == db::Entity::Type::Table;
        isView = entity->type() == db::Entity::Type::View;
        isFunction = entity->type() == db::Entity::Type::Function;
        isProcedure = entity->type() == db::Entity::Type::Procedure;
        isRoutine = isFunction || isProcedure;
        isTrigger = entity->type() == db::Entity::Type::Trigger;


        removeEntityTabsExcept(entity->type());
        if (isTable) {
            auto tableEntity = static_cast<db::TableEntity *>(entity);
            tableTab()->setTable(tableEntity);
        } else if (isView) {
            auto viewEntity = static_cast<db::ViewEntity *>(entity);
            viewTab()->setView(viewEntity);
        } else if (isRoutine) {
            auto routineEntity = static_cast<db::RoutineEntity *>(entity);
            routineTab()->setRoutine(routineEntity);
        } else if (isTrigger) {
            auto triggerEntity = static_cast<db::TriggerEntity *>(entity);
            triggerTab()->setTrigger(triggerEntity);
        }
    } else {
        removeTableTab();
        removeViewTab();
        removeRoutineTab();
        removeTriggerTab();
    }

    if (_model.hasDataTab()) {
        bool loadData = onDataTab();
        dataTab()->resetFilter();
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

    } else if (isTable || isView || isRoutine || isTrigger) {

        _rootTabs->setTabText((int)Tabs::Entity,
                              _model.titleForEntityTab());

        if (isRoutine) {
            _rootTabs->setTabIcon((int)Tabs::Entity,
                                  _model.iconForRoutineTab());
        }

        if ( !wasOnDataTab && !wasOnQueryTab ) {
            _rootTabs->setCurrentIndex((int)Tabs::Entity);
        }
    }

    if (_model.hasQueryTab()) {
        queryTab();
    }
}

bool CentralRightWidget::onGlobalRefresh()
{
    if (onHostTab()) {
        hostTab()->onGlobalRefresh();
        return true;
    } else if (onDatabaseTab()) {

    } else if (onEntityTab()) {

    } else if (onDataTab()) {
        dataTab()->refresh();
        return true;
    }

    return false;
}

void CentralRightWidget::onBeforeEntityEditing()
{
    meow::db::Entity * entity = _model.currentEntity();
    if (!entity) return;

    using Tabs = models::ui::CentralRightWidgetTabs;

    if (_model.hasEntityTab()) {
        if (entity->type() == db::Entity::Type::Table) {
            tableTab()->onBeforeEntityEditing();
        } else if (entity->type() == db::Entity::Type::View) {
            viewTab()->onBeforeEntityEditing();
        } else if (entity->type() == db::Entity::Type::Procedure
                   || entity->type() == db::Entity::Type::Function) {
            routineTab()->onBeforeEntityEditing();
        }
        // TODO: trigger
        _rootTabs->setCurrentIndex((int)Tabs::Entity);
    }
}

void CentralRightWidget::onEntityEdited(db::Entity * entity)
{
    if (_model.hasEntityTab()) {
        if (entity->type() == db::Entity::Type::Table
         || entity->type() == db::Entity::Type::View
         || entity->type() == db::Entity::Type::Procedure
         || entity->type() == db::Entity::Type::Function
         || entity->type() == db::Entity::Type::Trigger) {

            _rootTabs->setTabText(
                (int)models::ui::CentralRightWidgetTabs::Entity,
                _model.titleForEntityTab());

            if (_model.hasDataTab()) {
                dataTab()->invalidateData();
            }
        }
    }
}

void CentralRightWidget::onGlobalDataFilterPatternChanged(
        const QString & pattern, bool regexp)
{
    if (_model.hasDataTab() && onDataTab()) {
        dataTab()->setFilterPattern(pattern, regexp);
        _filterWidget->setRowCount(
                    dataTab()->totalRowCount(),
                    dataTab()->filterMatchedRowCount());
    }
}

bool CentralRightWidget::onHostTab() const
{
    return _rootTabs->currentIndex()
            == static_cast<int>(models::ui::CentralRightWidgetTabs::Host);
}

bool CentralRightWidget::onDatabaseTab() const
{
    return _rootTabs->currentIndex()
            == static_cast<int>(models::ui::CentralRightWidgetTabs::Database);
}

bool CentralRightWidget::onEntityTab() const
{
    return _rootTabs->currentIndex()
            == static_cast<int>(models::ui::CentralRightWidgetTabs::Entity);
}

bool CentralRightWidget::onDataTab() const
{
    return _rootTabs->currentIndex() == _model.indexForDataTab();
}

bool CentralRightWidget::onQueryTab() const
{
    return _rootTabs->currentIndex() == _model.indexForQueryTab();
}

void CentralRightWidget::createWidgets()
{
    // http://doc.qt.io/qt-5/qtwidgets-dialogs-tabdialog-example.html
    _rootTabs = new QTabWidget();

    connect(_rootTabs,
            &QTabWidget::currentChanged,
            this,
            &CentralRightWidget::rootTabChanged);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

    _filterWidget = new central_right::FilterWidget();
    _filterWidget->setVisible(showGlobalFilterPanel());
    layout->addWidget(_filterWidget);
    connect(_filterWidget, &central_right::FilterWidget::onFilterPatterChanged,
            this, &CentralRightWidget::onGlobalDataFilterPatternChanged);


}

void CentralRightWidget::rootTabChanged(int index)
{
    _filterWidget->setVisible(showGlobalFilterPanel());

    if (index < 0) return;

    try {
        if (onDataTab()) {
            dataTab()->loadData();

            bool showFilter = meow::app()->settings()
                    ->geometrySettings()->showFilterPanel();
            if (showFilter) {
                _filterWidget->setFilterPattern(
                            dataTab()->filterPattern(),
                            dataTab()->filterPatternIsRegexp());
            } else {
                _filterWidget->reset();
            }
            _filterWidget->setRowCount(
                        dataTab()->totalRowCount(),
                        dataTab()->filterMatchedRowCount());

        } else {
            _filterWidget->reset();
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

void CentralRightWidget::onDataTabDataChanged()
{
    if (onDataTab()) {
        _filterWidget->setRowCount(
                    dataTab()->totalRowCount(),
                    dataTab()->filterMatchedRowCount());
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

central_right::RoutineTab * CentralRightWidget::routineTab()
{
    if (!_routineTab) {
        _routineTab = new central_right::RoutineTab();
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Entity,
                             _routineTab,
                             _model.iconForRoutineTab(),
                             _model.titleForRoutineTab());
    }
    return _routineTab;
}

central_right::TriggerTab * CentralRightWidget::triggerTab()
{
    if (!_triggerTab) {
        _triggerTab = new central_right::TriggerTab();
        _rootTabs->insertTab((int)models::ui::CentralRightWidgetTabs::Entity,
                             _triggerTab,
                             QIcon(":/icons/trigger.png"),
                             _model.titleForTriggerTab());
    }
    return _triggerTab;
}

central_right::DataTab * CentralRightWidget::dataTab()
{
    if (!_dataTab) {
        _dataTab = new central_right::DataTab();

        // TODO: don't connect if filter is empty?

        connect(_dataTab->model(), &QAbstractItemModel::dataChanged,
                this, &CentralRightWidget::onDataTabDataChanged);

        connect(_dataTab->model(), &QAbstractItemModel::rowsInserted,
                this, &CentralRightWidget::onDataTabDataChanged);

        connect(_dataTab->model(), &QAbstractItemModel::rowsRemoved,
                this, &CentralRightWidget::onDataTabDataChanged);

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
    removeRoutineTab();
    removeTriggerTab();
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

bool CentralRightWidget::removeRoutineTab()
{
    if (removeTab(_routineTab)) {
        _routineTab = nullptr;
        return true;
    }
    return false;
}

bool CentralRightWidget::removeTriggerTab()
{
    if (removeTab(_triggerTab)) {
        _triggerTab = nullptr;
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
    if (except != db::Entity::Type::Function
            && except != db::Entity::Type::Procedure) {
        removeRoutineTab();
    }
    if (except != db::Entity::Type::Trigger) {
        removeTriggerTab();
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

bool CentralRightWidget::showGlobalFilterPanel() const
{
    if (_model.hasDataTab() && onDataTab()) {
        return meow::app()->settings()->geometrySettings()->showFilterPanel();
    }
    return false;
}

} // namespace meow
} // namespace ui
} // namespace main_window
