#include "user_manager.h"
#include "connection.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

#ifdef WITH_MYSQL

MySQLUserManager::MySQLUserManager(Connection * connection)
    :IUserManager(connection)
{

}

QString MySQLUserManager::currentUser() const
{
    if (_currentUser.isEmpty()) {
        _currentUser = _connection->getCell("SELECT CURRENT_USER()");
    }
    return _currentUser;
}

QStringList MySQLUserManager::allUsers() const
{
    if (!_currentUsers.isEmpty()) {
        return _currentUsers;
    }

    QString SQL = "SELECT CONCAT(`User`, '@', `Host`) FROM `mysql`.`user`";
    SQL += " WHERE `User`!='' ORDER BY `User`, `Host`";

    try {
        _currentUsers = _connection->getColumn(SQL);
    } catch (meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
            << "Failed to fetch users: " << ex.message();
    }

    return _currentUsers;
}

#endif // WITH_MYSQL


} // namespace db
} // namespace meow
