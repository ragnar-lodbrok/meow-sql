#include "right_widget.h"

namespace meow {
namespace ui {
namespace user_manager {

RightWidget::RightWidget(models::forms::UserManagementForm * form,
                         QWidget *parent) : QWidget(parent)
{
    createWidgets(form);
}

void RightWidget::createWidgets(models::forms::UserManagementForm * form)
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    _options = new OptionsWidget(form, this);
    mainLayout->addWidget(_options);

    _privileges = new PrivilegesWidget(form, this);
    mainLayout->addWidget(_privileges);

    this->setLayout(mainLayout);
}

void RightWidget::fillDataFromForm()
{
    _options->fillDataFromForm();
    _privileges->fillDataFromForm();
}

} // namespace user_manager
} // namespace ui
} // namespace
