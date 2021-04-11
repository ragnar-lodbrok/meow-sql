#include "mysql_user_editor.h"
#include "db/connection.h"
#include <QDebug>

namespace meow {
namespace db {

inline QString normalizeHost(const QString & host) {
    return host.isEmpty() ? "%" : host;
}

MySQLUserEditor::MySQLUserEditor(Connection * connection)
    : IUserEditor(connection)
{

}

bool MySQLUserEditor::edit(User * user, User * newData)
{
    bool changed = false;

    QString curHost = normalizeHost(user->host());
    QString newHost = normalizeHost(newData->host());

    bool userOrHostChanged = (curHost != newHost)
            || (user->username() != newData->username());

    if (userOrHostChanged) {
        const QList<UserPtr> & users = _connection->userManager()->userList();
        for (const UserPtr & userIt : users) {
            const QString userItHost = normalizeHost(userIt->host());
            if (userItHost == newHost
                    && userIt->username() == newData->username()) {
                throw db::Exception(
                            QString("User '%1'@'%2' already exists.")
                            .arg(newData->username())
                            .arg(newHost));
            }
        }
    }

    if (editLimits(user, newData)) {
        changed = true;
    }

    if (editPassword(user, newData)) {
        changed = true;
    }

    if (userOrHostChanged) {
        changed = true;
        if (_connection->serverVersionInt() >= 50002) {
            QString renameUserSQL = QString("RENAME USER %1@%2 TO %3@%4")
                .arg(_connection->escapeString(user->username()))
                .arg(_connection->escapeString(curHost))
                .arg(_connection->escapeString(newData->username()))
                .arg(_connection->escapeString(newHost));
            _connection->query(renameUserSQL);
        } else {
            Q_ASSERT(false);
            // TODO
        }
    }

    if (changed) {
        _connection->query("FLUSH PRIVILEGES");
    }

    return changed;
}

bool MySQLUserEditor::insert(User * user)
{
    Q_UNUSED(user);
    return false;
}

bool MySQLUserEditor::drop(User * user)
{
    Q_UNUSED(user);
    return false;
}

bool MySQLUserEditor::editLimits(User * user, User * newData)
{
    // https://dev.mysql.com/doc/refman/5.6/en/user-resources.html

    if (user->limitsAreEqual(newData)) {
        return false;
    }

    const QList<User::LimitType> & supportedLimits
            = _connection->userManager()->supportedLimitTypes();

    QMap<User::LimitType, QString> limitNames = {
        {User::LimitType::MaxQueriesPerHour,     "MAX_QUERIES_PER_HOUR"},
        {User::LimitType::MaxUpdatesPerHour,     "MAX_UPDATES_PER_HOUR"},
        {User::LimitType::MaxConnectionsPerHour, "MAX_CONNECTIONS_PER_HOUR"},
        {User::LimitType::MaxUserConnections,    "MAX_USER_CONNECTIONS"},
    };

    QStringList withClauses;

    for (const User::LimitType type : supportedLimits) {
        if (user->limit(type) != newData->limit(type)) {
            withClauses << (limitNames.value(type) + " "
                        + QString::number(newData->limit(type)));
        }
    }

    // The statement modifies only the limit value specified
    // and leaves the account otherwise unchanged:
    // > GRANT USAGE ON *.* TO 'francis'@'localhost'
    // ->     WITH MAX_QUERIES_PER_HOUR 100;

    if (withClauses.isEmpty()) {
        return false;
    }

    QString grantUsageWithLimits = QString("GRANT USAGE ON *.* TO %1@%2 WITH %3")
            .arg(_connection->escapeString(user->username()))
            .arg(_connection->escapeString(normalizeHost(user->host())))
            .arg(withClauses.join(' '));

    _connection->query(grantUsageWithLimits);

    return true;
}

bool MySQLUserEditor::editPassword(User * user, User * newData)
{
    if (newData->password().isEmpty()) {
        return false; // no new password - ignore
    }

    // mysql> SET PASSWORD FOR
    // -> 'jeffrey'@'localhost' = PASSWORD('mypass'); // < 5.7
    // SET PASSWORD FOR 'jeffrey'@'localhost' = 'auth_string'; // >= 5.7

    bool isMariaDB = false; // TODO
    bool useRawAuthString = (!isMariaDB
                             && _connection->serverVersionInt() >= 50706);

    QString escapedPassword = _connection->escapeString(newData->password());

    QString setPasswordSQL = QString("SET PASSWORD FOR %1@%2 = %3")
            .arg(_connection->escapeString(user->username()))
            .arg(_connection->escapeString(normalizeHost(user->host())))
            .arg(useRawAuthString
                 ? escapedPassword
                 : ("PASSWORD(" + escapedPassword + ")"));

    _connection->query(setPasswordSQL);

    return true;
}

} // namespace db
} // namespace meow
