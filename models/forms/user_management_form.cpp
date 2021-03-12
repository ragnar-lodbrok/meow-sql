#include "user_management_form.h"

namespace meow {
namespace models {
namespace forms {

UserManagementForm::UserManagementForm(db::SessionEntity * session,
                                       meow::db::IUserManager * userManager)
    : _session(session)
    , _userManager(userManager)
    , _selectedUser(nullptr)
{

}

void UserManagementForm::selectUser(const meow::db::UserPtr & selectedUser)
{
    if (selectedUser.get() == _selectedUser.get()) return;

    _selectedUser = selectedUser;

    if (_selectedUser) { // TODO: here?
        _userManager->loadPrivileges(_selectedUser);
    }

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

QString UserManagementForm::userHost() const
{
    return _selectedUser ? _selectedUser->host() : QString();
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

} // namespace forms
} // namespace models
} // namespace meow
