#include "user_management_form.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include <QDebug>

namespace meow {
namespace models {
namespace forms {

UserManagementForm::UserManagementForm(db::SessionEntity * session)
    : _session(session)
    , _userManager(session->connection()->userManager())
    , _selectedUser(nullptr)
    , _hasUnsavedChanges(false)
{

}

void UserManagementForm::selectUser(const meow::db::UserPtr & user)
{
    //if (user.get() == _sourceUser.get()) return;

    if (user) {
        _userManager->loadPrivileges(user);
    }

    //if (selectUser->isNew()) { // TODO
    //} else {

    _sourceUser = user; // hold a ref to source user for update
    _selectedUser.reset(user ? user->deepCopy() : nullptr); // copy to edit

    //}

    _repeatPassword = QString();

    setHasUnsavedChanges(false);

    emit selectedUserChanged();
}

QString UserManagementForm::userWarningMessage() const
{
    if (!_selectedUser) return QString();

    switch (_selectedUser->status()) {

    case db::User::Status::EmptyPassword:
        return tr("This user has an empty password.");

    case db::User::Status::InvalidPasswordLength:
        return tr("This user is inactive due to an invalid length of its"
                  " encrypted password. Please fix that in the %1 table.")
                .arg("mysql.user");

    case db::User::Status::SkipNameResolve:
        return tr("This user is inactive due to having a host name, "
                  "while the server runs with %1.")
                .arg("--skip-name-resolve");

    case db::User::Status::UnknownError:
        return tr("This user is inactive due to some unknown reason.");

    default:
        return QString();

    }
}

QString UserManagementForm::userName() const
{
    return _selectedUser ? _selectedUser->username() : QString();
}

void UserManagementForm::setUserName(const QString & userName)
{
    if (_selectedUser) {
        _selectedUser->setUsername(userName);
        setHasUnsavedChanges(true);
    }
}

QString UserManagementForm::userHost() const
{
    return _selectedUser ? _selectedUser->host() : QString();
}

void UserManagementForm::setUserHost(const QString & host)
{
    if (_selectedUser) {
        _selectedUser->setHost(host);
        setHasUnsavedChanges(true);
    }
}

QString UserManagementForm::password() const
{
    return _selectedUser ? _selectedUser->password() : QString();
}

void UserManagementForm::setPassword(const QString & password)
{
    if (_selectedUser) {
        _selectedUser->setPassword(password);
        setHasUnsavedChanges(true);
    }
}

void UserManagementForm::setRepeatPassword(const QString & password)
{
    _repeatPassword = password;
    setHasUnsavedChanges(true);
}

QList<meow::db::User::LimitType> UserManagementForm::supportedLimitTypes() const
{
    return _userManager->supportedLimitTypes();
}

QMap<db::User::LimitType, int> UserManagementForm::userLimits() const
{
    QMap<db::User::LimitType, int> limits;

    for (const auto limitType : _userManager->supportedLimitTypes()) {
        limits[limitType] = limitValue(limitType);
    }

    return limits;
}

QString UserManagementForm::limitName(db::User::LimitType limit) const
{
    switch (limit) {

    case db::User::LimitType::MaxQueriesPerHour:
        return tr("Queries per hour:");

    case db::User::LimitType::MaxUpdatesPerHour:
        return tr("Updates per hour:");

    case db::User::LimitType::MaxConnectionsPerHour:
        return tr("Connections per hour:");

    case db::User::LimitType::MaxUserConnections:
        return tr("Simultaneous connections:");

    default:
        Q_ASSERT(false);
        return QString();

    }
}

int UserManagementForm::limitValue(meow::db::User::LimitType limit) const
{
    return _selectedUser ? _selectedUser->limit(limit) : 0;
}

void UserManagementForm::setLimit(meow::db::User::LimitType limit, int value)
{
    if (_selectedUser) {
        _selectedUser->setLimit(limit, value);
        setHasUnsavedChanges(true);
    }
}

void UserManagementForm::save()
{
    if (_selectedUser && _hasUnsavedChanges) {

        // TODO: if _selectedUser->isNew() -> insert

        if (_selectedUser->password() != _repeatPassword) {
            throw db::Exception("Repeated password does not match first one.");
        }

        if (_session->connection()->userEditor()->edit(
                    _sourceUser.get(), _selectedUser.get())) {
            _selectedUser->setPassword(QString()); // TODO: better place?
            _userManager->updateUserData(_sourceUser, _selectedUser);

            selectUser(_sourceUser);
        }
    }
}

void UserManagementForm::discard()
{
    if (_sourceUser) {
        selectUser(_sourceUser);
    }
}

void UserManagementForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

} // namespace forms
} // namespace models
} // namespace meow
