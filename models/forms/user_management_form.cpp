#include "user_management_form.h"

namespace meow {
namespace models {
namespace forms {

UserManagementForm::UserManagementForm(meow::db::IUserManager * userManager)
    : _userManager(userManager)
    , _selectedUser(nullptr)
{

}

void UserManagementForm::selectUser(const meow::db::UserPtr & selectedUser)
{
    if (selectedUser.get() == _selectedUser.get()) return;

    _selectedUser = selectedUser;

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

QMap<db::User::LimitType, int> UserManagementForm::userLimits() const
{
    QMap<db::User::LimitType, int> limits;

    for (const auto limitType : _userManager->supportedLimitTypes()) {
        limits[limitType] = _selectedUser ? _selectedUser->limit(limitType) : 0;
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

} // namespace forms
} // namespace models
} // namespace meow
