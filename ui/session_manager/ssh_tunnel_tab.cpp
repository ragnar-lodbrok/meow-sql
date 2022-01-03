#include "ssh_tunnel_tab.h"

namespace meow {
namespace ui {
namespace session_manager {

SSHTunnelTab::SSHTunnelTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();

    // Listening: Judicator - Tomorrow's Sun
}

void SSHTunnelTab::createWidgets()
{
    _mainGridLayout = new QGridLayout();
    int row = 0;

    // SSH Host+port -----------------------------------------------------------

    _sshHostPortLabel = new QLabel(tr("SSH host + port:"));
    _mainGridLayout->addWidget(_sshHostPortLabel, row, 0);

    _sshHostEdit = new QLineEdit();
    _sshHostPortLabel->setBuddy(_sshHostEdit);
    _mainGridLayout->addWidget(_sshHostEdit, row, 1);
    connect(_sshHostEdit, &QLineEdit::textEdited,
            [=](const QString &newHost) {
                if (_form) {
                    _form->setSSHHost(newHost);
                }
            });

    _sshPortSpinBox = new QSpinBox();
    _sshPortSpinBox->setMinimum(0);
    _sshPortSpinBox->setMaximum(65535);
    _sshPortSpinBox->resize(70, _sshPortSpinBox->height());
    _mainGridLayout->addWidget(_sshPortSpinBox, row, 2);
    connect(_sshPortSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int newPort){
                if (_form) {
                    _form->setSSHPort(static_cast<quint16>(newPort));
                }
            });

    row++;

    // Username ----------------------------------------------------------------

    _usernameLabel = new QLabel(tr("Username:"));
    _mainGridLayout->addWidget(_usernameLabel, row, 0);

    _usernameEdit = new QLineEdit();
    _usernameLabel->setBuddy(_usernameEdit);
    _mainGridLayout->addWidget(_usernameEdit, row, 1, 1, 2);
    connect(_usernameEdit, &QLineEdit::textEdited,
            [=](const QString &newUser) {
                if (_form) {
                    _form->setSSHUser(newUser);
                }
            });
    row++;

    // Password ----------------------------------------------------------------

    // TODO: hide it for OpenSSH

    /*_passwordLabel = new QLabel(tr("Password:"));
    _mainGridLayout->addWidget(_passwordLabel, row, 0);

    _passwordEdit = new QLineEdit();
    _passwordLabel->setBuddy(_passwordEdit);
    _passwordEdit->setEchoMode(QLineEdit::Password);
    _mainGridLayout->addWidget(_passwordEdit, row, 1, 1, 2);
    connect(_passwordEdit, &QLineEdit::textEdited,
            [=](const QString &newPassword) {
                if (_form) {
                    _form->setSSHPassword(newPassword);
                }
            });
    row++;*/

    // Local port --------------------------------------------------------------

    _localPortLabel = new QLabel(tr("Local port:"));
    _mainGridLayout->addWidget(_localPortLabel, row, 0);

    _localPortSpinBox = new QSpinBox();
    _localPortLabel->setBuddy(_localPortSpinBox);
    _localPortSpinBox->setMinimum(0);
    _localPortSpinBox->setMaximum(65535);
    _localPortSpinBox->resize(70, _localPortSpinBox->height());

    QHBoxLayout * spinBoxLayout = new QHBoxLayout();
    spinBoxLayout->addWidget(_localPortSpinBox);
    spinBoxLayout->addStretch(1);
    _mainGridLayout->addLayout(spinBoxLayout, row, 1);
    connect(_localPortSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int newPort){
                if (_form) {
                    _form->setSSHLocalPort(static_cast<quint16>(newPort));
                }
            });

    row++;

    // -------------------------------------------------------------------------

    _mainGridLayout->setColumnMinimumWidth(0, 150);
    _mainGridLayout->setColumnStretch(1, 2);
    _mainGridLayout->setAlignment(Qt::AlignTop);
    this->setLayout(_mainGridLayout);
}

void SSHTunnelTab::setForm(presenters::ConnectionParametersForm * form)
{
    _form = form;
    fillDataFromForm();
}

void SSHTunnelTab::fillDataFromForm()
{
    if (!_form) {
        return;
    }

    _sshHostEdit->setText(_form->sshHost());
    _sshPortSpinBox->setValue(_form->sshPort());
    _usernameEdit->setText(_form->sshUser());
    //_passwordEdit->setText(_form->sshPassword());
    _localPortSpinBox->setValue(_form->sshLocalPort());

}

} // namespace session_manager
} // namespace ui
} // namespace meow
