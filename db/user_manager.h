#ifndef DB_USERMANAGER_H
#define DB_USERMANAGER_H

#include <QStringList>
#include <QMap>
#include <QSet>
#include <memory>

namespace meow {
namespace db {

class Connection;

class UserPrivilege
{
public:
    enum class Scope {
        Global,
        DatabaseLevel,
        TableLevel,
        TableColumnLevel,
        ProcedureLevel,
        FunctionLevel,
        Proxy // TODO
    };

    UserPrivilege(Scope scope = Scope::Global,
                  QString databaseName = QString(),
                  QString entityName = QString(),
                  QString fieldName = QString())
        : _scope(scope)
        , _databaseName(databaseName)
        , _entityName(entityName)
        , _fieldName(fieldName)
    {

    }

    Scope scope() const {
        return _scope;
    }

    QString databaseName() const {
        return _databaseName;
    }

    QString entityName() const {
        return _entityName;
    }

    QString fieldName() const { // e.g. table column
        return _fieldName;
    }

    void grantPrivilege(const QString & name) {
        _grantedPrivileges << name;
    }

    bool invokePrivilege(const QString & name) {
        return _grantedPrivileges.remove(name);
    }

    bool hasPrivilege(const QString & name) const {
        return _grantedPrivileges.contains(name);
    }

private:
    Scope _scope;
    QString _databaseName;
    QString _entityName;
    QString _fieldName;
    QSet<QString> _grantedPrivileges;
};

using UserPrivilegePtr = std::shared_ptr<UserPrivilege>;

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

    void clearPrivileges() {
        _limits.clear();
        _privileges.clear();
    }

    QList<UserPrivilegePtr> & privileges() {
        return _privileges;
    }

    UserPrivilegePtr globalPrivileges() const {
        for (const auto& privilege : _privileges) {
            if (privilege->scope() == UserPrivilege::Scope::Global) {
                return privilege;
            }
        }
        return nullptr;
    }

private:
    Status _status;
    QString _username;
    QString _host;
    QString _password;
    QMap<LimitType, int> _limits;
    QList<UserPrivilegePtr> _privileges;
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
    virtual void loadPrivileges(const UserPtr & user) = 0;
    virtual QStringList supportedPrivilegesForScope(
            UserPrivilege::Scope scope) const = 0;
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_USERMANAGER_H
