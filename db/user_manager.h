#ifndef DB_USERMANAGER_H
#define DB_USERMANAGER_H

#include <QStringList>

namespace meow {
namespace db {

class Connection;

class IUserManager
{
public:
    IUserManager(Connection * connection)
        : _connection(connection) {}
    virtual ~IUserManager() {}

    virtual QString currentUser() const = 0;
    virtual QStringList allUsers() const { return {}; };
protected:
    Connection * _connection;
};

#ifdef WITH_MYSQL

class MySQLUserManager : public IUserManager
{
public:
    MySQLUserManager(Connection * connection);

    virtual QString currentUser() const override;
    virtual QStringList allUsers() const override;
private:
    mutable QString _currentUser;
    mutable QStringList _currentUsers;
};

#endif // WITH_MYSQL

} // namespace db
} // namespace meow

#endif // DB_USERMANAGER_H
