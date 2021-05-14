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

    if (editPrivileges(user, newData)) {
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

UserPasswordRequirements MySQLUserEditor::passwordRequirements() const
{
    UserPasswordRequirements reqs;

    reqs.minLength = sessionVariableAsInt("validate_password_length");
    reqs.minNumberCount
            = sessionVariableAsInt("validate_password_number_count");
    reqs.minMixedCaseCount
            = sessionVariableAsInt("validate_password_mixed_case_count");
    reqs.minSpecialCharCount
            = sessionVariableAsInt("validate_password_special_char_count");

    return reqs;
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

bool MySQLUserEditor::editPrivileges(User * user, User * newData)
{
    // TODO: probably we should sort privileges by scope to revoke lower
    // at first and grant higher at first?

    bool changed = false;

    for (const UserPrivilegePtr & oldPriv : user->privileges()) {
        UserPrivilegePtr newPriv = newData->privilegeById(oldPriv->id());
        if (newPriv) {
            if (newPriv->grantedPrivileges() != oldPriv->grantedPrivileges()) {
                editPrivileges(oldPriv, newPriv, user); // changed
                changed = true;
            }
        } else {
            Q_ASSERT(false); // privilege object can't be removed in UI
        }
    }

    for (const UserPrivilegePtr & newPriv : newData->privileges()) {
        UserPrivilegePtr oldPriv = user->privilegeById(newPriv->id());
        if (!oldPriv) {
            grantPrivileges(newPriv, newPriv->grantedPrivileges(), user);
            changed = true;
        }
    }

    return changed;
}

void MySQLUserEditor::editPrivileges(const UserPrivilegePtr & oldPriv,
                                     const UserPrivilegePtr & newPriv,
                                     User * user)
{
    // Listening: NINGEN ISU - Heartless Scat

    Q_ASSERT(oldPriv->id() == newPriv->id());
    Q_ASSERT(oldPriv->grantedPrivileges() != newPriv->grantedPrivileges());

    const QSet<QString> & oldList = oldPriv->grantedPrivileges();
    const QSet<QString> & newList = newPriv->grantedPrivileges();

    const QString userHost = normalizeHost(user->host());

    QStringList revokedPrivileges;
    for (const QString & privName : oldList) {
        if (!newList.contains(privName)) {
            if (privName == "GRANT") {
                revokedPrivileges << "GRANT OPTION";
            } else if (oldPriv->scope()
                       == UserPrivilege::Scope::TableColumnLevel) {
                revokedPrivileges
                    << (privName
                        + " ("
                        + _connection->quoteIdentifier(oldPriv->fieldName())
                        +")");
            } else {
                revokedPrivileges << privName;
            }
        }
    }

    if (!revokedPrivileges.isEmpty()) {
        revoke(revokedPrivileges,
               privilegeLevelSQL(oldPriv),
               userHostSQL(user));
    }

    QSet<QString> grantList;
    for (const QString & privName : newList) {
        if (!oldList.contains(privName)) {
            grantList << privName;
        }
    }

    if (!grantList.isEmpty()) {
        grantPrivileges(oldPriv, grantList, user);
    }
}

void MySQLUserEditor::grantPrivileges(const UserPrivilegePtr & priv,
                                      const QSet<QString> & privList,
                                      User * user)
{
    QStringList grantedPrivileges;
    for (const QString & privName : privList) {
        if (priv->scope() == UserPrivilege::Scope::TableColumnLevel) {
            grantedPrivileges << (privName
                        + " ("
                        + _connection->quoteIdentifier(priv->fieldName())
                        +")");
        } else {
            grantedPrivileges << privName;
        }
    }

    if (grantedPrivileges.isEmpty()) return;

    if (priv->scope() != UserPrivilege::Scope::TableColumnLevel) {

        QStringList newListSorted = privList.values();
        newListSorted.sort(); // for QList::operator==

        QStringList availableListSorted
                = _connection->userManager()->supportedPrivilegesForScope(
                    priv->scope());
        availableListSorted.sort(); // for QList::operator==

        // TODO: compare full granted list (not granted this time!) to all
        // supported for this scope and use ALL if equal
        bool allGranted = newListSorted == availableListSorted;

        if (allGranted) {
            grantedPrivileges.clear();
            grantedPrivileges << "ALL";
        }
    }

    grant(grantedPrivileges,
          privilegeLevelSQL(priv),
          userHostSQL(user));

}

void MySQLUserEditor::revoke(const QStringList & privList,
                             const QString & onObject,
                             const QString & fromUser)
{
    QString SQL = QString("REVOKE %1 ON %2 FROM %3")
            .arg(privList.join(", "))
            .arg(onObject)
            .arg(fromUser);
    _connection->query(SQL);
}

void MySQLUserEditor::grant(const QStringList & privList,
                            const QString & onObject,
                            const QString & toUser)
{
    bool hasGrantOption = privList.contains("GRANT")
                       || privList.contains("ALL");

    QStringList grantList = privList;
    if (hasGrantOption) {
        grantList.removeAll("GRANT");
    }

    QString grantString = grantList.isEmpty() ? "USAGE" : grantList.join(", ");

    QString withString = hasGrantOption ? "WITH GRANT OPTION" : QString();

    QString SQL = QString("GRANT %1 ON %2 TO %3 %4")
            .arg(grantString)
            .arg(onObject)
            .arg(toUser)
            .arg(withString).trimmed();
    _connection->query(SQL);
}

QString MySQLUserEditor::privilegeLevelSQL(const UserPrivilegePtr & priv) const
{
    const QString quotedDb = _connection->quoteIdentifier(priv->databaseName());
    const QString quotedEntity = _connection->quoteIdentifier(priv->entityName());

    switch (priv->scope()) {
    case UserPrivilege::Scope::Global:
        return "*.*";
    case UserPrivilege::Scope::DatabaseLevel:
        return quotedDb + ".*"; // TODO: escape _ to \_ to avoid wildcart
    case UserPrivilege::Scope::TableLevel:
    case UserPrivilege::Scope::FunctionLevel:
    case UserPrivilege::Scope::ProcedureLevel:
    case UserPrivilege::Scope::TableColumnLevel: {

        QString objType;
        if (_connection->serverVersionInt() >= 50006) {
            if (priv->scope() == UserPrivilege::Scope::TableLevel
                || priv->scope() == UserPrivilege::Scope::TableColumnLevel ) {
                objType = "TABLE ";
            } else if (priv->scope() == UserPrivilege::Scope::FunctionLevel) {
                objType = "FUNCTION ";
            } else if (priv->scope() == UserPrivilege::Scope::ProcedureLevel) {
                objType = "PROCEDURE ";
            }
        }

        return objType + quotedDb + '.' + quotedEntity;
    }
    case UserPrivilege::Scope::Proxy:
    default:
        Q_ASSERT(false);
        return QString();
    }
}

QString MySQLUserEditor::userHostSQL(const User * user) const
{
    return _connection->escapeString(user->username()) + '@' +
           _connection->escapeString(normalizeHost(user->host()));
}

int MySQLUserEditor::sessionVariableAsInt(const QString & variableName) const
{
    SessionVariables * sessionVars = _connection->variables();
    sessionVars->fetch();
    QString value = sessionVars->value(variableName, true);
    return value.isEmpty() ? 0 : value.toInt();
}

} // namespace db
} // namespace meow
