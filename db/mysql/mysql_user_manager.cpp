#include "mysql_user_manager.h"
#include "db/connection.h"
#include "helpers/logger.h"
#include "db/query.h"
#include <QDebug>

namespace meow {
namespace db {

bool hostRequiresNameResolve(const QString & host)
{
    QRegularExpression regexp;

    // TODO: plain copy from Heidi - is it correct?
    QString pattern
        = R"(^(localhost|[\d\.\/\:]+|.*%.*|[\w\d]{4}\:.*)$)";

    regexp.setPattern(pattern);

    QRegularExpressionMatch match = regexp.match(host);

    return match.hasMatch();
}

MySQLUserManager::MySQLUserManager(Connection * connection)
    :IUserManager(connection)
{

}

QString MySQLUserManager::currentUserName() const
{
    if (_currentUserName.isEmpty()) {
        _currentUserName = _connection->getCell("SELECT CURRENT_USER()");
    }
    return _currentUserName;
}

QStringList MySQLUserManager::allUserNames() const
{
    if (!_currentUserNames.isEmpty()) {
        return _currentUserNames;
    }

    QString SQL = "SELECT CONCAT(`User`, '@', `Host`) FROM `mysql`.`user`";
    SQL += " WHERE `User`!='' ORDER BY `User`, `Host`";

    try {
        _currentUserNames = _connection->getColumn(SQL);
    } catch (meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
            << "Failed to fetch users: " << ex.message();
    }

    return _currentUserNames;
}

const QList<UserPtr> & MySQLUserManager::userList(bool refresh) const
{
    if (!refresh && !_userList.isEmpty()) {
        return _userList;
    }

    _userList.clear();

    _connection->query("FLUSH PRIVILEGES"); // do we need it?

    QStringList userTableColumns = this->userTableColumns();

    bool hasPassword = userTableColumns.contains("password",
                                                 Qt::CaseInsensitive);
    bool hasAuthString = userTableColumns.contains("authentication_string",
                                                   Qt::CaseInsensitive);

    QString passwordExpr;

    if (hasPassword && !hasAuthString) {
        passwordExpr = "password";
    } else if (!hasPassword && hasAuthString) {
        passwordExpr = "authentication_string";
    } else if (hasPassword && hasAuthString) {
        passwordExpr = "IF(LENGTH(password)>0, password, authentication_string)";
    } else {
        throw db::Exception(QObject::tr("No password hash column available"));
    }

    QString usersSQL = "SELECT " + _connection->quoteIdentifier("user") + ", "
            + _connection->quoteIdentifier("host") + ", "
            + passwordExpr + " AS "
            + _connection->quoteIdentifier("password") + " "
            + " FROM " + _connection->quoteIdentifier("mysql") + "."
            + _connection->quoteIdentifier("user");


    QueryPtr query = _connection->getResults(usersSQL);

    std::size_t indexOfUser = query->indexOfColumn("user");
    std::size_t indexOfHost = query->indexOfColumn("host");
    std::size_t indexOfPassword = query->indexOfColumn("password");

    while (query->isEof() == false) {

        UserPtr user = std::make_shared<User>();

        user->setUsername(query->curRowColumn(indexOfUser));
        user->setHost(query->curRowColumn(indexOfHost));
        user->setPassword(query->curRowColumn(indexOfPassword));

        if (user->password().length() == 0) {
            user->setStatus(User::Status::EmptyPassword);
        }

        if (isValidPasswordHashLen(user->password()) == false) {
            user->setStatus(User::Status::InvalidPasswordLength);
        } else if (skipNameResolve() && hostRequiresNameResolve(user->host())) {
            user->setStatus(User::Status::SkipNameResolve);
        }
        _userList.append(user);

        query->seekNext();
    }

    return _userList;
}

bool MySQLUserManager::skipNameResolve() const
{
    _connection->variables()->fetch();
    QString skipNameResolveVar
            = _connection->variables()->value("skip_name_resolve");
    return skipNameResolveVar.toUpper() == "ON";
}

bool MySQLUserManager::isValidPasswordHashLen(const QString & passwordHash) const
{
    if (_connection->serverVersionInt() >= 80000) {
        // MySQL 8 has predefined length of hashed passwords only with
        // mysql_native_password plugin enabled users
        return true;
    }

    int len = passwordHash.length();

    // TODO: is 0 valid len?
    return len == 0 || len == 16 || len == 41;
}

QList<User::LimitType> MySQLUserManager::supportedLimitTypes() const {
    QList<User::LimitType> list;

    if (_connection->serverVersionInt() >= 40002) {
        list << User::LimitType::MaxQueriesPerHour;
        list << User::LimitType::MaxUpdatesPerHour;
        list << User::LimitType::MaxConnectionsPerHour;
    }

    if (_connection->serverVersionInt() >= 50003) {
        list << User::LimitType::MaxUserConnections;
    }
    return list;
}

void MySQLUserManager::loadPrivileges(const UserPtr & user)
{
    user->clearPrivileges();

    // SHOW CREATE USER is available since MySQL 5.7, so we can't use it for
    // nonprivilege properties on older, so let's use `mysql`.`user`

    QMap<QString, User::LimitType> limitColumns = {
        { "max_questions" , User::LimitType::MaxQueriesPerHour },
        { "max_updates" , User::LimitType::MaxUpdatesPerHour },
        { "max_connections" , User::LimitType::MaxConnectionsPerHour },
        { "max_user_connections" , User::LimitType::MaxUserConnections },
    };

    QStringList queryUserColumns;
    QStringList availableUserColumns = userTableColumns();
    QStringList wantedUserColumns;
    wantedUserColumns << limitColumns.keys();

    for (const QString & wanted : wantedUserColumns) {
        if (availableUserColumns.contains(wanted)) {
            queryUserColumns << wanted;
        }
    }

    if (queryUserColumns.isEmpty()) {
        return;
    }

    QString userRowSQL = "SELECT "
        + _connection->quoteIdentifiers(queryUserColumns).join(", ")
        + " FROM " + _connection->quoteIdentifier("mysql") + "."
        + _connection->quoteIdentifier("user")
        + QString("WHERE `Host` = %1 AND `User` = %2")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));


    QStringList userRow = _connection->getRow(userRowSQL);

    if (userRow.isEmpty()) {
        return;
    }

    int columnIndex = 0;
    for (const QString & columnName : queryUserColumns) {

        const QString & columnValue = userRow.at(columnIndex);

        if (limitColumns.contains(columnName)) {
            User::LimitType limitType = limitColumns.value(columnName);
            user->setLimit(limitType, columnValue.toInt());
        }

        ++columnIndex;
    }
}

QStringList MySQLUserManager::userTableColumns() const
{
    // stupid cache
    if (_userTableColumns.isEmpty()) {
        _userTableColumns = _connection->getColumn(
                    "SHOW COLUMNS FROM `mysql`.`user`");
    }
    return _userTableColumns;
}

} // namespace db
} // namespace meow
