#ifndef DB_MYSQL_USER_MANAGER_H
#define DB_MYSQL_USER_MANAGER_H

#include "db/user_manager.h"

namespace meow {
namespace db {

class MySQLUserManager : public IUserManager
{
public:
    MySQLUserManager(Connection * connection);

    virtual QString currentUserName() const override;
    virtual QStringList allUserNames() const override;
    virtual const QList<UserPtr> & userList(bool refresh = false) const override;
    virtual QList<User::LimitType> supportedLimitTypes() const override {
        return { User::LimitType::MaxQueriesPerHour,
                 User::LimitType::MaxUpdatesPerHour,
                 User::LimitType::MaxConnectionsPerHour,
                 User::LimitType::MaxUserConnections
        };
    }
private:

    bool skipNameResolve() const;
    bool isValidPasswordHashLen(const QString & passwordHash) const;

    mutable QString _currentUserName;
    mutable QStringList _currentUserNames;
    mutable QList<UserPtr> _userList;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_USER_MANAGER_H
