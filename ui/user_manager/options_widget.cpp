#include "options_widget.h"

namespace meow {
namespace ui {
namespace user_manager {

OptionsWidget::OptionsWidget(models::forms::UserManagementForm * form,
                             QWidget * parent)
    : QWidget(parent)
{
    createWidgets(form);
}

void OptionsWidget::createWidgets(models::forms::UserManagementForm * form)
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

    _credentialsTab = new CredentialsTab(form, this);
    _limitationsTab = new LimitationsTab(form, this);

    using Tabs = models::ui::UserOptionsWidgetModel::Tabs;

    _rootTabs->addTab(_credentialsTab,
                      _model.tabTitle(Tabs::Credentials));

    _rootTabs->addTab(_limitationsTab,
                      _model.tabTitle(Tabs::Limitations));

}

} // namespace user_manager
} // namespace ui
} // namespace meow
