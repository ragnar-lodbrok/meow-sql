#ifndef MODELS_FORMS_USER_MANAGEMENT_FORM_H
#define MODELS_FORMS_USER_MANAGEMENT_FORM_H

#include <QObject>
#include "db/user_manager.h"
#include "db/entity/session_entity.h"
#include "ui/models/users_table_model.h"

namespace meow {
namespace ui {
namespace presenters {

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

    models::UsersTableModel * tableModel() {
        return &_tableModel;
    }

    void loadData();

    void selectUser(const meow::db::UserPtr & user);
    meow::db::UserPtr selectedUser() const {
        return _selectedUser;
    }
    bool hasUser() const {
        return _selectedUser != nullptr;
    }
    bool isNewUser() const {
        return _selectedUser && _selectedUser->isNew();
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
    void discard(bool newUserOnly = false);

    void deleteSelectedUser();

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

    QStringList passwordRequirements() const;

    QStringList randomPasswords(int length, int count) const;

    QVector<int> randomPasswordsLengths() const;

    QString fullUserAccount() const;

    bool canSave() const { return hasUser() && hasUnsavedChanges(); }
    bool canDiscard() const { return canSave(); }
    bool canAddObject() const { return hasUser(); }
    bool canAddUser() const { return true; }
    bool canDeleteSelectedUser() const { return hasUser(); }
    bool canCloneSelectedUser() const { return hasUser(); }

private:
    meow::db::SessionEntity * _session;
    meow::db::IUserManager * _userManager;
    meow::db::UserPtr _selectedUser;
    meow::db::UserPtr _sourceUser;

    QString _repeatPassword;

    bool _hasUnsavedChanges;

    models::UsersTableModel _tableModel;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MODELS_FORMS_USER_MANAGEMENT_FORM_H
