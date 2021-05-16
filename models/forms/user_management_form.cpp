#include "user_management_form.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include "helpers/random_password_generator.h"
#include <QDebug>

namespace meow {
namespace models {
namespace forms {

UserManagementForm::UserManagementForm(meow::db::SessionEntity * session)
    : QObject(nullptr)
    , _session(session)
    , _userManager(session->connection()->userManager())
    , _selectedUser(nullptr)
    , _hasUnsavedChanges(false)
    , _tableModel(this)
{

}

void UserManagementForm::loadData()
{
    _userManager->refresh();
    _tableModel.setUserManager(_userManager);
}

void UserManagementForm::selectUser(const meow::db::UserPtr & user)
{
    if (user && !user->isNew()) {
        _userManager->loadPrivileges(user);
    }

    if (user && user->isNew()) {
        _sourceUser = nullptr;
        _selectedUser = user; // take ownership
    } else {
        _sourceUser = user; // hold a ref to source user for update
        _selectedUser.reset(user ? user->deepCopy() : nullptr); // copy to edit
    }

    _repeatPassword = QString();

    setHasUnsavedChanges(user && user->isNew());

    emit selectedUserChanged();
}

QString UserManagementForm::userWarningMessage() const
{
    if (!_selectedUser) return QString();

    switch (_selectedUser->status()) {

    case meow::db::User::Status::EmptyPassword:
        return tr("This user has an empty password.");

    case meow::db::User::Status::InvalidPasswordLength:
        return tr("This user is inactive due to an invalid length of its"
                  " encrypted password. Please fix that in the %1 table.")
                .arg("mysql.user");

    case meow::db::User::Status::SkipNameResolve:
        return tr("This user is inactive due to having a host name, "
                  "while the server runs with %1.")
                .arg("--skip-name-resolve");

    case meow::db::User::Status::UnknownError:
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

QMap<meow::db::User::LimitType, int> UserManagementForm::userLimits() const
{
    QMap<meow::db::User::LimitType, int> limits;

    for (const auto limitType : _userManager->supportedLimitTypes()) {
        limits[limitType] = limitValue(limitType);
    }

    return limits;
}

QString UserManagementForm::limitName(meow::db::User::LimitType limit) const
{
    switch (limit) {

    case meow::db::User::LimitType::MaxQueriesPerHour:
        return tr("Queries per hour:");

    case meow::db::User::LimitType::MaxUpdatesPerHour:
        return tr("Updates per hour:");

    case meow::db::User::LimitType::MaxConnectionsPerHour:
        return tr("Connections per hour:");

    case meow::db::User::LimitType::MaxUserConnections:
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

        if (_selectedUser->password() != _repeatPassword) {
            throw meow::db::Exception("Repeated password does not match first one.");
        }

        if (_selectedUser->isNew()) {
            if (_session->connection()->userEditor()->insert(
                        _selectedUser.get())) {
                _selectedUser->setIsNew(false);
                _selectedUser->setPassword(QString());
                _userManager->setUserDataChanged(_selectedUser);
                selectUser(_selectedUser);
            }
        } else {
            if (_session->connection()->userEditor()->edit(
                        _sourceUser.get(), _selectedUser.get())) {
                _selectedUser->setPassword(QString()); // TODO: better place?
                _userManager->updateUserData(_sourceUser, _selectedUser);

                selectUser(_sourceUser);
            }
        }

    }
}

void UserManagementForm::discard()
{
    if (_sourceUser) {
        selectUser(_sourceUser);
    } else if (_selectedUser && _selectedUser->isNew()) {
        auto selectedUser = _selectedUser;
        selectUser(nullptr);
        _tableModel.deleteUser(selectedUser);
    }
}

void UserManagementForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

QStringList UserManagementForm::passwordRequirements() const
{
    QStringList list;

    meow::db::UserPasswordRequirements reqs
            = _session->connection()->userEditor()->passwordRequirements();

    if (reqs.minLength > 0) {
        list << tr("%1 or more characters long").arg(reqs.minLength);
    }
    if (reqs.minMixedCaseCount > 0) {
        list << tr("%1 mixed case character(s)").arg(reqs.minMixedCaseCount);
    }
    if (reqs.minNumberCount > 0) {
        list << tr("%1 number(s)").arg(reqs.minNumberCount);
    }
    if (reqs.minSpecialCharCount > 0) {
        list << tr("%1 special character(s)").arg(reqs.minSpecialCharCount);
    }

    return list;
}

QStringList UserManagementForm::randomPasswords(int length, int count) const
{
    meow::helpers::RandomPasswordGenerator generator;

    QStringList passwords;
    passwords.reserve(count);

    while (count-- > 0) {
        passwords << generator.run(length);
    }

    return passwords;
}

QVector<int> UserManagementForm::randomPasswordsLengths() const
{
    QVector<int> lengths;

    meow::db::UserPasswordRequirements reqs
            = _session->connection()->userEditor()->passwordRequirements();

    for (int len : {6,8,10,12,15,30}) {
        if (reqs.minLength == 0 || len >= reqs.minLength) {
            lengths << len;
        }
    }

    return lengths;
}


} // namespace forms
} // namespace models
} // namespace meow
