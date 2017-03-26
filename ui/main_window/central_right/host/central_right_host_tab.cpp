#include "central_right_host_tab.h"
#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

HostTab::HostTab(QWidget *parent) : QWidget(parent)
{
    createRootTabs();
}

void HostTab::createRootTabs()
{
    _rootTabs = new QTabWidget();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Databases --------------------------

    _databasesTab = new HostDatabasesTab();
    _rootTabs->addTab(_databasesTab, QIcon(":/icons/database.png"), _model.titleForDatabasesTab());
}

void HostTab::setCurrentEntity(meow::db::SessionEntity * curEntity)
{
    bool sessionChanged = _model.setCurrentEntity(curEntity);
    if (sessionChanged) {
        onSessionChanged();
    }
}

void HostTab::onSessionChanged()
{
    _rootTabs->setTabText(Tabs::Databases, _model.titleForDatabasesTab());
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
