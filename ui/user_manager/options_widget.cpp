#include "options_widget.h"

namespace meow {
namespace ui {
namespace user_manager {

OptionsWidget::OptionsWidget(presenters::UserManagementForm * form,
                             QWidget * parent)
    : QWidget(parent)
{
    createWidgets(form);
}

void OptionsWidget::createWidgets(presenters::UserManagementForm * form)
{
    _rootTabs = new QTabWidget();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

    _credentialsTab = new CredentialsTab(form, this);
    _limitationsTab = new LimitationsTab(form, this);

    using Tabs = presenters::UserOptionsWidgetModel::Tabs;

    _rootTabs->addTab(_credentialsTab,
                      _model.tabTitle(Tabs::Credentials));

    _rootTabs->addTab(_limitationsTab,
                      _model.tabTitle(Tabs::Limitations));

}

void OptionsWidget::fillDataFromForm()
{
    _credentialsTab->fillDataFromForm();
    _limitationsTab->fillDataFromForm();
}

void OptionsWidget::onAddUserClicked()
{
    _credentialsTab->onAddUserClicked();
}

} // namespace user_manager
} // namespace ui
} // namespace meow
