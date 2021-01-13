#include "cr_routine_info.h"
#include "cr_routine_info_options_tab.h"
#include "cr_routine_info_parameters_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

RoutineInfo::RoutineInfo(models::forms::RoutineForm * form,
                         QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createTabs();
}

void RoutineInfo::createTabs()
{
    _rootTabs = new QTabWidget();

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

    _optionsTab = new routine_info::OptionsTab(_form, this);
    _parametersTab = new routine_info::ParametersTab(_form, this);

    using RoutineInfoTabs = models::ui::RoutineInfoWidgetTabs;

    _rootTabs->addTab(_optionsTab,
                      _tabsModel.tabIcon(RoutineInfoTabs::Options),
                      _tabsModel.tabTitle(RoutineInfoTabs::Options));

    _rootTabs->addTab(_parametersTab,
                      _tabsModel.tabIcon(RoutineInfoTabs::Parameters),
                      _tabsModel.tabTitle(RoutineInfoTabs::Parameters));
}

void RoutineInfo::refreshData()
{
    _optionsTab->refreshData();
    _parametersTab->refreshData();
}

void RoutineInfo::validateControls()
{
    _optionsTab->validateControls();
    _parametersTab->validateControls();
}

void RoutineInfo::onBeforeEntityEditing()
{
    _optionsTab->onBeforeEntityEditing();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
