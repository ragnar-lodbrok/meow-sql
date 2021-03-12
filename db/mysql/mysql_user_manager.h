#ifndef DB_MYSQL_USER_MANAGER_H
#define DB_MYSQL_USER_MANAGER_H

#include "db/user_manager.h"
#include <QHash>

namespace meow {
namespace db {

class MySQLUserManager : public IUserManager
{
public:
    MySQLUserManager(Connection * connection);

    virtual QString currentUserName() const override;
    virtual QStringList allUserNames() const override;
    virtual const QList<UserPtr> & userList(bool refresh = false) const override;
    virtual QList<User::LimitType> supportedLimitTypes() const override;
    virtual void loadPrivileges(const UserPtr & user) override;
    virtual QStringList supportedPrivilegesForScope(
            UserPrivilege::Scope scope) const override {
        initScopePrivileges();
        return _scopePrivileges.value(scope);
    }
    PrivilegeType typeOfPrivilege(const QString & privilegeName) const override;
private:

    bool skipNameResolve() const;
    bool isValidPasswordHashLen(const QString & passwordHash) const;

    QStringList userTableColumns() const;
    QStringList dbTableColumns() const;

    void initScopePrivileges() const;
    void loadFromUserTable(const UserPtr & user);
    void loadFromDBTable(const UserPtr & user);
    void loadFromTablePriv(const UserPtr & user);
    void loadFromProcsPriv(const UserPtr & user);
    void loadFromColumnsPriv(const UserPtr & user);

    mutable QString _currentUserName;
    mutable QStringList _currentUserNames;
    mutable QList<UserPtr> _userList;
    mutable QStringList _userTableColumns;
    mutable QStringList _dbTableColumns;
    mutable QMap<UserPrivilege::Scope, QStringList> _scopePrivileges;
    mutable QHash<QString, PrivilegeType> _privilegeTypes;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_USER_MANAGER_H
