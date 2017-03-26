#include "central_right_widget.h"
#include "db/entity/entity.h"
#include <QDebug>


namespace meow {
namespace ui {
namespace main_window {

CentralRightWidget::CentralRightWidget(QWidget *parent)
    : QWidget(parent),
      _model()
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
            entity->findParentOfType(db::Entity::Type::Session)
        );
        hostTab()->setCurrentEntity(sessionEntity);
        _rootTabs->setTabText(models::ui::CentralRightWidgetTabs::Host, _model.titleForHostTab());
    }

    if (entity->type() == db::Entity::Type::Session) {
        _rootTabs->setCurrentIndex(models::ui::CentralRightWidgetTabs::Host);
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

    _rootTabs->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

}

central_right::HostTab * CentralRightWidget::hostTab()
{
    if (!_hostTab) {
        _hostTab = new central_right::HostTab();
        _rootTabs->insertTab(models::ui::CentralRightWidgetTabs::Host, _hostTab, QIcon(":/icons/host.png"), _model.titleForHostTab());
    }

    return _hostTab;
}

} // namespace meow
} // namespace ui
} // namespace main_window
