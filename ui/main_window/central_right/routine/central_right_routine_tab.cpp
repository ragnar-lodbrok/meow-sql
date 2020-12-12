#include "central_right_routine_tab.h"
#include "cr_routine_body.h"
#include "cr_routine_info.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

RoutineTab::RoutineTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void RoutineTab::setRoutine(meow::db::RoutineEntity * routine)
{
    _form.setRoutine(routine);
    _info->refreshData();
    _body->refreshData();
}

void RoutineTab::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _info = new RoutineInfo(&_form, this);
    _info->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _info->setMinimumSize(QSize(300, 200));

    _body = new RoutineBody(&_form, this);
    _body->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _body->setMinimumSize(QSize(300, 150));

    _mainSplitter->addWidget(_info);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_body);
    _mainSplitter->setStretchFactor(1, 2);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
