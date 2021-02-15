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

    _connection->query("FLUSH PRIVILEGES");

    QStringList userTableColumns = _connection->getColumn(
                "SHOW COLUMNS FROM `mysql`.`user`");

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

} // namespace db
} // namespace meow
