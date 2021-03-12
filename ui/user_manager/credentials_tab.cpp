#include "credentials_tab.h"
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

CredentialsTab::CredentialsTab(models::forms::UserManagementForm *form,
                               QWidget * parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}

void CredentialsTab::createWidgets()
{
    QGridLayout * mainGridLayout = new QGridLayout();
    int row = 0;

    _usernameLabel = new QLabel(tr("User name:"));
    mainGridLayout->addWidget(_usernameLabel, row, 0);

    _usernameEdit = new QLineEdit();
    _usernameLabel->setBuddy(_usernameEdit);
    mainGridLayout->addWidget(_usernameEdit, row, 1);

    ++row;

    _hostLabel = new QLabel(tr("From host:"));
    mainGridLayout->addWidget(_hostLabel, row, 0);

    _hosteEdit = new QLineEdit();
    _hostLabel->setBuddy(_hosteEdit);
    mainGridLayout->addWidget(_hosteEdit, row, 1);

    mainGridLayout->setColumnMinimumWidth(0, 100);
    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 3);
    mainGridLayout->setAlignment(Qt::AlignTop);

    this->setLayout(mainGridLayout);
}

void CredentialsTab::fillDataFromForm()
{
    _usernameLabel->setEnabled(_form->hasUser());
    _hostLabel->setEnabled(_form->hasUser());
    _usernameEdit->setEnabled(_form->hasUser());
    _hosteEdit->setEnabled(_form->hasUser());

    _usernameEdit->blockSignals(true);
    _usernameEdit->setText(_form->userName());
    _usernameEdit->blockSignals(false);


    _hosteEdit->blockSignals(true);
    _hosteEdit->setText(_form->userHost());
    _hosteEdit->blockSignals(false);
}

} // namespace user_manager
} // namespace ui
} // namespace meow
