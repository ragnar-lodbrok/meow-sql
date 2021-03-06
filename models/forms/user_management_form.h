#ifndef MODELS_FORMS_USER_MANAGEMENT_FORM_H
#define MODELS_FORMS_USER_MANAGEMENT_FORM_H

#include <QObject>
#include "db/user_manager.h"

namespace meow {
namespace models {
namespace forms {

class UserManagementForm : public QObject
{
    Q_OBJECT
public:
    UserManagementForm(meow::db::IUserManager * userManager);

    meow::db::IUserManager * userManager() const {
        return _userManager;
    }

    void selectUser(const meow::db::UserPtr & selectedUser);
    meow::db::UserPtr selectedUser() const {
        return _selectedUser;
    }
    bool hasUser() const {
        return _selectedUser != nullptr;
    }
    Q_SIGNAL void selectedUserChanged();

    QString userWarningMessage() const;

    QString userName() const;
    QString userHost() const;


    QList<meow::db::User::LimitType> supportedLimitTypes() const;
    QMap<meow::db::User::LimitType, int> userLimits() const;

    QString limitName(meow::db::User::LimitType limit) const;
    inline int limitValue(meow::db::User::LimitType limit) const;

private:
    meow::db::IUserManager * _userManager;
    meow::db::UserPtr _selectedUser;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_USER_MANAGEMENT_FORM_H
