#include "central_widget.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {

CentralWidget::CentralWidget(models::db::EntitiesTreeModel * dbEntitiesTreeModel,
                             QWidget * parent)
    :QWidget(parent),
     _dbEntitiesTreeModel(dbEntitiesTreeModel)
{
    createMainLayout();
}

CentralWidget::~CentralWidget()
{
    saveGeometryToSettings();
}

void CentralWidget::saveGeometryToSettings()
{
    QSettings settings;

    settings.setValue("ui/main_window/center_widget/main_hsplitter",
                      _mainHorizontalSplitter->saveState());

    settings.setValue("ui/main_window/center_widget/main_vsplitter",
                      _mainVerticalSplitter->saveState());

}

void CentralWidget::loadGeometryFromSettings()
{
    QSettings settings;

    _mainHorizontalSplitter->restoreState(
        settings.value("ui/main_window/center_widget/main_hsplitter")
                .toByteArray());

    _mainVerticalSplitter->restoreState(
        settings.value("ui/main_window/center_widget/main_vsplitter")
                .toByteArray());
}

void CentralWidget::createMainLayout()
{

    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _mainVerticalSplitter = new QSplitter(Qt::Vertical, this);
    _mainLayout->addWidget(_mainVerticalSplitter);

    createCenterLayout();

    QWidget * mainTopVerticalContainer = new QWidget(this);
    mainTopVerticalContainer->setMinimumSize(QSize(200, 100));
    mainTopVerticalContainer->setLayout(_mainHorizLayout);

    _mainVerticalSplitter->addWidget(mainTopVerticalContainer);
    _mainVerticalSplitter->setStretchFactor(0, 5);

    _mainBottomWidget = new CentralBottomWidget(this);
    _mainBottomWidget->setMinimumSize(QSize(200, 40));
    _mainBottomWidget->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Maximum);

    auto settings = meow::app()->settings()->geometrySettings();
    toggleBottomWidget(settings->showSQLLog());
    connect(settings,
            &settings::Geometry::showSQLLogChanged,
            this,
            &CentralWidget::toggleBottomWidget);

    _mainVerticalSplitter->addWidget(_mainBottomWidget);
    _mainVerticalSplitter->setStretchFactor(1, 0);
    _mainBottomWidget->resize(QSize(200, 100));

    _mainVerticalSplitter->setChildrenCollapsible(false);

    loadGeometryFromSettings();
}

void CentralWidget::createCenterLayout()
{
    _mainHorizLayout = new QHBoxLayout();
    _mainHorizLayout->setContentsMargins(0, 0, 0, 0);

    _mainHorizontalSplitter = new QSplitter(this);
    _mainHorizontalSplitter->setChildrenCollapsible(false);


    _mainLeftWidget = new CentralLeftWidget(_dbEntitiesTreeModel);
    _mainLeftWidget->setMinimumSize(QSize(200, 400));

    _mainRightWidget = new CentralRightWidget();
    _mainRightWidget->setMinimumSize(QSize(350, 400));

    _mainHorizontalSplitter->addWidget(_mainLeftWidget);
    _mainHorizontalSplitter->setStretchFactor(0, 0);
    _mainHorizontalSplitter->addWidget(_mainRightWidget);
    _mainHorizontalSplitter->setStretchFactor(1, 5);

    _mainHorizLayout->addWidget(_mainHorizontalSplitter);
}

void CentralWidget::setActiveDBEntity(db::Entity * entity, bool select)
{
    if (select) {
        _mainLeftWidget->selectEntity(entity);
    }
    _mainRightWidget->setActiveDBEntity(entity);
}

void CentralWidget::onCreatingNewEntity(db::Entity * entity)
{
    _mainLeftWidget->selectEntity(entity); // clears prev selection
    _mainRightWidget->setActiveDBEntity(entity);
    _mainRightWidget->onBeforeEntityEditing();
}

void CentralWidget::toggleBottomWidget(bool show)
{
    if (show) {
        _mainBottomWidget->show();
    } else {
        _mainBottomWidget->hide();
    }
}

} // namespace main_window
} // namespace ui
} // namespace meow
