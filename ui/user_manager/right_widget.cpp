#include "right_widget.h"
#include "window.h"

namespace meow {
namespace ui {
namespace user_manager {

RightWidget::RightWidget(models::forms::UserManagementForm * form,
                         Window * window) : QWidget(window)
{
    createWidgets(form, window);
}

void RightWidget::createWidgets(models::forms::UserManagementForm * form,
                                Window * window)
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 0, 0, 0);

    _options = new OptionsWidget(form, this);
    mainLayout->addWidget(_options, 0);

    _privileges = new PrivilegesWidget(form, window);
    mainLayout->addWidget(_privileges, 1);

    this->setLayout(mainLayout);
}

void RightWidget::fillDataFromForm()
{
    _options->fillDataFromForm();
    _privileges->fillDataFromForm();
}

void RightWidget::validateControls()
{
    _privileges->validateControls();
}

void RightWidget::onAddUserClicked()
{
    _options->onAddUserClicked();
}

} // namespace user_manager
} // namespace ui
} // namespace
