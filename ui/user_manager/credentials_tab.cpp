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
    int row = 0; // ---------------------------

    _usernameLabel = new QLabel(tr("User name:"));
    mainGridLayout->addWidget(_usernameLabel, row, 0);

    _usernameEdit = new QLineEdit();
    _usernameLabel->setBuddy(_usernameEdit);
    connect(_usernameEdit, &QLineEdit::textChanged,
            [=](const QString &name) {
                _form->setUserName(name);
            });
    mainGridLayout->addWidget(_usernameEdit, row, 1);

    ++row; // ------------------------

    _hostLabel = new QLabel(tr("From host:"));
    mainGridLayout->addWidget(_hostLabel, row, 0);

    _hostEdit = new QLineEdit();
    _hostLabel->setBuddy(_hostEdit);
    connect(_hostEdit, &QLineEdit::textChanged,
            [=](const QString &host) {
                _form->setUserHost(host);
            });
    mainGridLayout->addWidget(_hostEdit, row, 1);

    ++row; // -----------------------

    _passwordLabel = new QLabel(tr("Password:"));
    mainGridLayout->addWidget(_passwordLabel, row, 0);

    _passwordEdit = new QLineEdit();
    _passwordLabel->setBuddy(_passwordEdit);
    QStringList passwordRequirements = _form->passwordRequirements();
    if (!passwordRequirements.isEmpty()) {
        _passwordEdit->setToolTip(passwordRequirements.join("\n"));
    }
    connect(_passwordEdit, &QLineEdit::textChanged,
            [=](const QString &password) {
                _form->setPassword(password);
            });
    QAction * showPasswordsAction = _passwordEdit->addAction(
                QIcon(":/icons/dropdown_highlight.png"),
                QLineEdit::TrailingPosition);
    connect(showPasswordsAction, &QAction::triggered,
            this, &CredentialsTab::onShowPasswordsAction);
    mainGridLayout->addWidget(_passwordEdit, row, 1);

    ++row; // -----------------------

    _repeatPasswordLabel = new QLabel(tr("Repeat password:"));
    mainGridLayout->addWidget(_repeatPasswordLabel, row, 0);

    _repeatPasswordEdit = new QLineEdit();
    _repeatPasswordLabel->setBuddy(_repeatPasswordEdit);
    connect(_repeatPasswordEdit, &QLineEdit::textChanged,
            [=](const QString &password) {
                _form->setRepeatPassword(password);
            });
    mainGridLayout->addWidget(_repeatPasswordEdit, row, 1);

    // --------------------------

    mainGridLayout->setColumnMinimumWidth(0, 100);
    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 3);
    mainGridLayout->setAlignment(Qt::AlignTop);

    this->setLayout(mainGridLayout);
}

void CredentialsTab::onShowPasswordsAction()
{
    // Listening: Wolfheart - The Hunt

    QMenu lengthMenu;

    for (int length : _form->randomPasswordsLengths()) {

        QMenu * lenSubMenu = lengthMenu.addMenu( // owns result
            QString("%1 characters").arg(length));

        for (const QString & password : _form->randomPasswords(length, 10)) {
            QAction * passwordAction = new QAction(
                        password,
                        lenSubMenu);
            passwordAction->setData(password);
            lenSubMenu->addAction(passwordAction);
        }
    }

    int menuXPos = _passwordEdit->width() - lengthMenu.sizeHint().width();
    int menuYPos = _passwordEdit->height() - 1;

    QAction * executedAction
            = lengthMenu.exec(_passwordEdit->mapToGlobal(
                                 QPoint(menuXPos, menuYPos)));
    if (!executedAction) return;

    QString password = executedAction->data().toString();
    if (password.isEmpty()) return;

    _passwordEdit->setText(password);
    _repeatPasswordEdit->setText(password);
}

void CredentialsTab::fillDataFromForm()
{
    _usernameLabel->setEnabled(_form->hasUser());
    _hostLabel->setEnabled(_form->hasUser());
    _usernameEdit->setEnabled(_form->hasUser());
    _hostEdit->setEnabled(_form->hasUser());
    _passwordLabel->setEnabled(_form->hasUser());
    _passwordEdit->setEnabled(_form->hasUser());
    _repeatPasswordLabel->setEnabled(_form->hasUser());
    _repeatPasswordEdit->setEnabled(_form->hasUser());

    _usernameEdit->blockSignals(true);
    _usernameEdit->setText(_form->userName());
    _usernameEdit->blockSignals(false);

    _hostEdit->blockSignals(true);
    _hostEdit->setText(_form->userHost());
    _hostEdit->blockSignals(false);

    _passwordEdit->blockSignals(true);
    _passwordEdit->setText(_form->password());
    _passwordEdit->blockSignals(false);

    _repeatPasswordEdit->blockSignals(true);
    _repeatPasswordEdit->setText(_form->repeatPassword());
    _repeatPasswordEdit->blockSignals(false);
}

} // namespace user_manager
} // namespace ui
} // namespace meow
