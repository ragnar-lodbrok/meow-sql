#ifndef MODELS_FORMS_USER_MANAGEMENT_FORM_H
#define MODELS_FORMS_USER_MANAGEMENT_FORM_H

#include <QObject>
#include "db/user_manager.h"

namespace meow {

namespace db {
class SessionEntity;
}

namespace models {
namespace forms {

class UserManagementForm : public QObject
{
    Q_OBJECT
public:
    UserManagementForm(meow::db::SessionEntity * session);

    meow::db::SessionEntity * session() const {
        return _session;
    }

    meow::db::IUserManager * userManager() const {
        return _userManager;
    }

    void selectUser(const meow::db::UserPtr & user);
    meow::db::UserPtr selectedUser() const {
        return _selectedUser;
    }
    bool hasUser() const {
        return _selectedUser != nullptr;
    }
    Q_SIGNAL void selectedUserChanged();

    meow::db::UserPtr sourceUser() const {
        return _sourceUser;
    }

    QString userWarningMessage() const;

    QString userName() const;
    void setUserName(const QString & userName);

    QString userHost() const;
    void setUserHost(const QString & host);

    QString password() const;
    void setPassword(const QString & password);

    QString repeatPassword() const {
        return _repeatPassword;
    }
    void setRepeatPassword(const QString & password);

    QList<meow::db::User::LimitType> supportedLimitTypes() const;
    QMap<meow::db::User::LimitType, int> userLimits() const;

    QString limitName(meow::db::User::LimitType limit) const;
    int limitValue(meow::db::User::LimitType limit) const;
    void setLimit(meow::db::User::LimitType limit, int value);

    void save();
    void discard();

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

private:
    meow::db::SessionEntity * _session;
    meow::db::IUserManager * _userManager;
    meow::db::UserPtr _selectedUser;
    meow::db::UserPtr _sourceUser;

    QString _repeatPassword;

    bool _hasUnsavedChanges;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_USER_MANAGEMENT_FORM_H
