#include "central_right_trigger_tab.h"
#include "cr_trigger_options.h"
#include "cr_trigger_body.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TriggerTab::TriggerTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void TriggerTab::setTrigger(db::TriggerEntity * trigger)
{
    _form.setTrigger(trigger);
    _options->refreshData();
    _body->refreshData();
}

void TriggerTab::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _options = new TriggerOptions(&_form, this);
    _options->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _options->setMinimumSize(QSize(300, 120));

    _body = new TriggerBody(&_form, this);
    _body->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _body->setMinimumSize(QSize(300, 150));

    _mainSplitter->addWidget(_options);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_body);
    _mainSplitter->setStretchFactor(1, 2);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
