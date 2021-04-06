#ifndef DB_USERMANAGER_H
#define DB_USERMANAGER_H

#include <QStringList>
#include <QMap>
#include <QSet>
#include <QObject>
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

    int grantedPrivilegeCount() const {
        return _grantedPrivileges.count();
    }

    UserPrivilege * deepCopy() const {
        UserPrivilege * copy = new UserPrivilege();
        *copy = *this;
        return copy;
    }

private:
    Scope _scope;
    QString _databaseName;
    QString _entityName;
    QString _fieldName;
    QSet<QString> _grantedPrivileges;
};

using UserPrivilegePtr = std::shared_ptr<UserPrivilege>;

enum class PrivilegeType {
    None,
    Read,
    Write,
    Admin
};

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

    bool limitsAreEqual(const User * another) const {
        return _limits == another->_limits;
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

    void copyDataFrom(const User * user) {
        this->_status = user->_status;
        this->_username = user->_username;
        this->_host = user->_host;
        this->_password = user->_password;
        this->_limits = user->_limits;
        this->_privileges.clear();
        this->_privileges.reserve(user->_privileges.size());
        for (const UserPrivilegePtr & privPtr : user->_privileges) {
            this->_privileges.push_back(
                        std::shared_ptr<UserPrivilege>(privPtr->deepCopy()));
        }
    }

    User * deepCopy() const {
        User * copy = new User();
        copy->copyDataFrom(this);
        return copy;
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

class IUserManager : public QObject
{
    Q_OBJECT
public:
    IUserManager(Connection * connection)
        : QObject()
        , _connection(connection) {}
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

    virtual PrivilegeType typeOfPrivilege(const QString & privilegeName) const {
        Q_UNUSED(privilegeName);
        return PrivilegeType::None;
    }

    virtual void updateUserData(const UserPtr & user,
                                const UserPtr & userData) {
        user->copyDataFrom(userData.get());
        emit userDataChanged(user);
    }

    void refresh() {
        this->userList(true);
    }

    // 1  - all granted
    // -1 - none granted
    // 0  - some granted
    int allPrivilegesGranted(const UserPrivilegePtr & privileges)
    {
        int allCount
                = supportedPrivilegesForScope(privileges->scope()).size();
        int grantedCount = privileges->grantedPrivilegeCount();

        if (grantedCount == 0 && allCount != 0) {
            return -1;
        }

        return (grantedCount == allCount) ? 1 : 0;
    }

    Q_SIGNAL void userDataChanged(const UserPtr & user);

protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_USERMANAGER_H
