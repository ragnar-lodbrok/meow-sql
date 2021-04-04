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

} // namespace db
} // namespace meow
