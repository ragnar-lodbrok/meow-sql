#ifndef DB_USERMANAGER_H
#define DB_USERMANAGER_H

#include <QStringList>
#include <QMap>
#include <memory>

namespace meow {
namespace db {

class Connection;

class User
{
public:

    enum class Status
    {
        Normal,
        EmptyPassword,
        InvalidPasswordLength,
        SkipNameResolve,
        UnknownError
    };

    enum class LimitType {
        MaxQueriesPerHour,
        MaxUpdatesPerHour,
        MaxConnectionsPerHour,
        MaxUserConnections
    };

    User() : _status(Status::Normal) {}

    Status status() const { return _status; }
    QString username() const { return _username; }
    QString host() const { return _host; }
    QString password() const { return _password; }

    void setStatus(const Status status) { _status = status; }
    void setUsername(const QString & username) { _username = username; }
    void setHost(const QString & host) { _host = host; }
    void setPassword(const QString & password) { _password = password; }

    int limit(LimitType type) const {
        return _limits.value(type, 0);
    }

    void setLimit(LimitType type, int value) {
        _limits[type] = value;
    }

private:
    Status _status;
    QString _username;
    QString _host;
    QString _password;
    QMap<LimitType, int> _limits;
};

using UserPtr = std::shared_ptr<User>;

class IUserManager
{
public:
    IUserManager(Connection * connection)
        : _connection(connection) {}
    virtual ~IUserManager() {}

    virtual QString currentUserName() const = 0;
    virtual QStringList allUserNames() const { return {}; }
    virtual const QList<UserPtr> & userList(bool refresh = false) const = 0;
    virtual QList<User::LimitType> supportedLimitTypes() const {
        return {};
    }
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_USERMANAGER_H
