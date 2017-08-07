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
            if (_databaseTab) {
                _rootTabs->removeTab(models::ui::CentralRightWidgetTabs::Database);
                delete _databaseTab;
                _databaseTab = nullptr;
            }
        }
    }

    if (_model.hasDataTab()) {
        bool loadData = onDataTab();
        dataTab()->setDBEntity(entity, loadData);
        _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Data,
                              _model.titleForDataTab());
    } else if (_dataTab) {
        _rootTabs->removeTab(models::ui::CentralRightWidgetTabs::Data);
        delete _dataTab;
        _dataTab = nullptr;
    }

    if (entity->type() == db::Entity::Type::Session) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Host);
    } else if (entity->type() == db::Entity::Type::Database) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Database);
    } else if (entity->type() == db::Entity::Type::Table) {
        tableTab();
        _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Entity,
                              _model.titleForTableTab());
        if ( ! onDataTab()) {
            _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Entity);
        }
    }
}

bool CentralRightWidget::onDataTab() const
{
    return _rootTabs->currentIndex() == models::ui::CentralRightWidgetTabs::Data;
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
        _rootTabs->insertTab(models::ui::CentralRightWidgetTabs::Data,
                             _dataTab,
                             QIcon(":/icons/data.png"),
                             _model.titleForDataTab());
    }

    return _dataTab;
}

} // namespace meow
} // namespace ui
} // namespace main_window
