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
        QString password = query->curRowColumn(indexOfPassword);
        //user->setPassword(password); // don't store password (not required)

        if (password.length() == 0) {
            user->setStatus(User::Status::EmptyPassword);
        }

        if (isValidPasswordHashLen(password) == false) {
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

    user->privileges() << std::make_shared<UserPrivilege>(
                              UserPrivilege::Scope::Global);

    // SHOW CREATE USER is available since MySQL 5.7, so we can't use it for
    // nonprivilege properties on older, so let's use `mysql`.`user`

    // Heidi uses SHOW GRANTS FOR, but parsing SQL is a pain, so I'll just read
    // `mysql`.`user` and other tables (mysql server does the same)

    loadFromUserTable(user); // `mysql`.`user`
    loadFromDBTable(user); // `mysql`.`db`
    loadFromTablePriv(user); // `mysql`.`tables_priv`
    loadFromProcsPriv(user); // `mysql`.`procs_priv`
    loadFromColumnsPriv(user); // `mysql`.`columns_priv`
}

void MySQLUserManager::loadFromUserTable(const UserPtr & user)
{
    QMap<QString, User::LimitType> limitColumns = {
        { "max_questions" , User::LimitType::MaxQueriesPerHour },
        { "max_updates" , User::LimitType::MaxUpdatesPerHour },
        { "max_connections" , User::LimitType::MaxConnectionsPerHour },
        { "max_user_connections" , User::LimitType::MaxUserConnections },
    };

    QMap<QString, QString> privilegeColumns = {
        {"Select_priv",           "SELECT"},
        {"Insert_priv",           "INSERT"},
        {"Update_priv",           "UPDATE"},
        {"Delete_priv",           "DELETE"},
        {"Index_priv",            "INDEX"},
        {"Alter_priv",            "ALTER"},
        {"Create_priv",           "CREATE"},
        {"Drop_priv",             "DROP"},
        {"Grant_priv",            "GRANT"},
        {"Create_view_priv",      "CREATE VIEW"},
        {"Show_view_priv",        "SHOW VIEW"},
        {"Create_routine_priv",   "CREATE ROUTINE"},
        {"Alter_routine_priv",    "ALTER ROUTINE"},
        {"Execute_priv",          "EXECUTE"},
        {"Trigger_priv",          "TRIGGER"},
        {"Event_priv",            "EVENT"},
        {"Create_tmp_table_priv", "CREATE TEMPORARY TABLES"},
        {"Lock_tables_priv",      "LOCK TABLES"},
        {"References_priv",       "REFERENCES"},
        // global only:
        {"Reload_priv",           "RELOAD"},
        {"Shutdown_priv",         "SHUTDOWN"},
        {"Process_priv",          "PROCESS"},
        {"File_priv",             "FILE"},
        {"Show_db_priv",          "SHOW DATABASES"},
        {"Super_priv",            "SUPER"},
        {"Repl_slave_priv",       "REPLICATION SLAVE"},
        {"Repl_client_priv",      "REPLICATION CLIENT"},
        {"Create_user_priv",      "CREATE USER"},
        {"Create_tablespace_priv","CREATE TABLESPACE"},
    };

    QStringList queryUserColumns;
    QStringList availableUserColumns = userTableColumns();
    QStringList wantedUserColumns;
    wantedUserColumns << privilegeColumns.keys();
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
        + QString(" WHERE `Host` = %1 AND `User` = %2")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));


    QStringList userRow = _connection->getRow(userRowSQL);

    if (userRow.isEmpty()) {
        return;
    }

    UserPrivilegePtr globalPrivileges = user->globalPrivileges();
    Q_ASSERT(globalPrivileges != nullptr);

    int columnIndex = 0;
    for (const QString & columnName : queryUserColumns) {

        const QString & columnValue = userRow.at(columnIndex);

        if (privilegeColumns.contains(columnName)) {

            if (columnValue == "Y") {
                const QString & privilegeName
                        = privilegeColumns.value(columnName);
                globalPrivileges->grantPrivilege(privilegeName);
            }

        } else if (limitColumns.contains(columnName)) {

            User::LimitType limitType = limitColumns.value(columnName);
            user->setLimit(limitType, columnValue.toInt());
        }

        ++columnIndex;
    }
}

void MySQLUserManager::loadFromDBTable(const UserPtr & user)
{
    QMap<QString, QString> privilegeColumns = {
        {"Select_priv",           "SELECT"},
        {"Insert_priv",           "INSERT"},
        {"Update_priv",           "UPDATE"},
        {"Delete_priv",           "DELETE"},
        {"Index_priv",            "INDEX"},
        {"Alter_priv",            "ALTER"},
        {"Create_priv",           "CREATE"},
        {"Drop_priv",             "DROP"},
        {"Grant_priv",            "GRANT"},
        {"Create_view_priv",      "CREATE VIEW"},
        {"Show_view_priv",        "SHOW VIEW"},
        {"Create_routine_priv",   "CREATE ROUTINE"},
        {"Alter_routine_priv",    "ALTER ROUTINE"},
        {"Execute_priv",          "EXECUTE"},
        {"Trigger_priv",          "TRIGGER"},
        {"Event_priv",            "EVENT"},
        {"Create_tmp_table_priv", "CREATE TEMPORARY TABLES"},
        {"Lock_tables_priv",      "LOCK TABLES"},
        {"References_priv",       "REFERENCES"},
    };

    QStringList queryColumns = { "Db" }; // take db name
    QStringList availableColumns = dbTableColumns();
    QStringList wantedColumns;
    wantedColumns << privilegeColumns.keys();

    for (const QString & wanted : wantedColumns) {
        if (availableColumns.contains(wanted)) {
            queryColumns << wanted;
        }
    }

    if (queryColumns.size() == 1) {
        return;
    }

    QString dbRowSQL = "SELECT "
        + _connection->quoteIdentifiers(queryColumns).join(", ")
        + " FROM " + _connection->quoteIdentifier("mysql") + "."
        + _connection->quoteIdentifier("db")
        + QString(" WHERE `Host` = %1 AND `User` = %2")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));


    QList<QStringList> dbRows = _connection->getRows(dbRowSQL);

    for (const QStringList & dbRow : dbRows) {

        const QString & dbName = dbRow.at(0);

        auto dbPrivilege = std::make_shared<UserPrivilege>(
                                      UserPrivilege::Scope::DatabaseLevel,
                                      dbName, dbName);

        user->privileges() << dbPrivilege;

        int columnIndex = 0;

        for (const QString & columnName : queryColumns) {

            const QString & columnValue = dbRow.at(columnIndex);

            if (privilegeColumns.contains(columnName)) {

                if (columnValue == "Y") {

                    const QString & privilegeName
                        = privilegeColumns.value(columnName);

                    dbPrivilege->grantPrivilege(privilegeName);
                }

            }

            ++columnIndex;
        }
    }
}

void MySQLUserManager::loadFromTablePriv(const UserPtr & user)
{
    QStringList queryColumns = { "Db", "Table_name", "Table_priv" };
    
    QString tablePrivRowSQL = "SELECT "
        + _connection->quoteIdentifiers(queryColumns).join(", ")
        + " FROM " + _connection->quoteIdentifier("mysql") + "."
        + _connection->quoteIdentifier("tables_priv")
        + QString(" WHERE `Host` = %1 AND `User` = %2"
                  " AND LENGTH(`Table_priv`) > 0")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));

    QList<QStringList> tablePrivRows = _connection->getRows(tablePrivRowSQL);

    for (const QStringList & tablePrivRow : tablePrivRows) {

        const QString & dbName = tablePrivRow.at(0);
        const QString & tableName = tablePrivRow.at(1);
        const QString & tablePrivilegesString = tablePrivRow.at(2);

        if (tablePrivilegesString.isEmpty()) {
            continue; // TODO: looks like we can ignore it and read `Column_priv`
            // from `columns_priv`
        }
        bool isView = this->isView(dbName, tableName);

        QStringList privilegesName = tablePrivilegesString.split(',');

        auto tablePrivilege = std::make_shared<UserPrivilege>(
                                      UserPrivilege::Scope::TableLevel,
                                      dbName, tableName, QString(), isView);

        for (const QString & privName : privilegesName) {
            tablePrivilege->grantPrivilege(privName.toUpper());
        }

        user->privileges() << tablePrivilege;
    }
}

void MySQLUserManager::loadFromProcsPriv(const UserPtr & user)
{
    QStringList queryColumns = { "Db", "Routine_name",
                                 "Routine_type", "Proc_priv" };

    QString procsPrivRowSQL = "SELECT "
        + _connection->quoteIdentifiers(queryColumns).join(", ")
        + " FROM " + _connection->quoteIdentifier("mysql") + "."
        + _connection->quoteIdentifier("procs_priv")
        + QString(" WHERE `Host` = %1 AND `User` = %2")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));

    QList<QStringList> procsPrivRows = _connection->getRows(procsPrivRowSQL);

    for (const QStringList & procPrivRow : procsPrivRows) {

        const QString & dbName = procPrivRow.at(0);
        const QString & routineName = procPrivRow.at(1);
        const QString & routineType = procPrivRow.at(2);
        const QString & privString = procPrivRow.at(3);

        QStringList privilegesName = privString.split(',');

        auto routinePrivilege = std::make_shared<UserPrivilege>(
            routineType == "PROCEDURE" ? UserPrivilege::Scope::ProcedureLevel
                                       : UserPrivilege::Scope::FunctionLevel,
            dbName, routineName);

        for (const QString & privName : privilegesName) {
            routinePrivilege->grantPrivilege(privName.toUpper());
        }

        user->privileges() << routinePrivilege;
    }
}

void MySQLUserManager::loadFromColumnsPriv(const UserPtr & user)
{
    QStringList queryColumns = { "Db", "Table_name",
                                 "Column_name", "Column_priv" };

    QString columnPrivRowSQL = "SELECT "
        + _connection->quoteIdentifiers(queryColumns).join(", ")
        + " FROM " + _connection->quoteIdentifier("mysql") + "."
        + _connection->quoteIdentifier("columns_priv")
        + QString(" WHERE `Host` = %1 AND `User` = %2")
            .arg(_connection->escapeString(user->host()))
            .arg(_connection->escapeString(user->username()));

    QList<QStringList> columnPrivRows = _connection->getRows(columnPrivRowSQL);

    for (const QStringList & columnPrivRow : columnPrivRows) {

        const QString & dbName = columnPrivRow.at(0);
        const QString & tableName = columnPrivRow.at(1);
        const QString & columnName = columnPrivRow.at(2);
        const QString & privString = columnPrivRow.at(3);

        QStringList privilegesName = privString.split(',');

        auto columnPrivilege = std::make_shared<UserPrivilege>(
                                      UserPrivilege::Scope::TableColumnLevel,
                                      dbName, tableName, columnName);

        for (const QString & privName : privilegesName) {
            columnPrivilege->grantPrivilege(privName.toUpper());
        }

        user->privileges() << columnPrivilege;
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

QStringList MySQLUserManager::dbTableColumns() const
{
    if (_dbTableColumns.isEmpty()) {
        _dbTableColumns = _connection->getColumn(
                    "SHOW COLUMNS FROM `mysql`.`db`");
    }
    return _dbTableColumns;
}

void MySQLUserManager::initScopePrivileges() const
{
    if (!_scopePrivileges.isEmpty()) return;

    // keep all lists sorted!

    // read global -------------------------------------------------------------
    QStringList readGlobalPriveleges;
    if (_connection->serverVersionInt() >= 40002) { // Who runs such old?
        readGlobalPriveleges << "EXECUTE";
    }
    readGlobalPriveleges << "PROCESS";
    readGlobalPriveleges << "SELECT";
    if (_connection->serverVersionInt() >= 40002) {
        readGlobalPriveleges << "SHOW DATABASES";
    }
    if (_connection->serverVersionInt() >= 50001) {
        readGlobalPriveleges << "SHOW VIEW";
    }

    // write global ------------------------------------------------------------
    QStringList writeGlobalPriveleges;
    writeGlobalPriveleges << "ALTER";
    if (_connection->serverVersionInt() >= 50003) {
        writeGlobalPriveleges << "ALTER ROUTINE";
    }
    writeGlobalPriveleges << "CREATE";
    if (_connection->serverVersionInt() >= 50003) {
        writeGlobalPriveleges << "CREATE ROUTINE";
    }
    if (_connection->serverVersionInt() >= 50404) {
        writeGlobalPriveleges << "CREATE TABLESPACE";
    }
    if (_connection->serverVersionInt() >= 40002) {
        writeGlobalPriveleges << "CREATE TEMPORARY TABLES";
    }
    if (_connection->serverVersionInt() >= 50001) {
        writeGlobalPriveleges << "CREATE VIEW";
    }
    writeGlobalPriveleges << "DELETE";
    writeGlobalPriveleges << "DROP";
    if (_connection->serverVersionInt() >= 50106) {
        writeGlobalPriveleges << "EVENT";
    }
    writeGlobalPriveleges << "INDEX";
    writeGlobalPriveleges << "INSERT";
    writeGlobalPriveleges << "REFERENCES";
    if (_connection->serverVersionInt() >= 50106) {
        writeGlobalPriveleges << "TRIGGER";
    }
    writeGlobalPriveleges << "UPDATE";

    // admin global ------------------------------------------------------------
    QStringList adminGlobalPriveleges;
    if (_connection->serverVersionInt() >= 50003) {
        adminGlobalPriveleges << "CREATE USER";
    }
    adminGlobalPriveleges << "FILE";
    adminGlobalPriveleges << "GRANT";
    if (_connection->serverVersionInt() >= 40002) {
        adminGlobalPriveleges << "LOCK TABLES";
    }
    adminGlobalPriveleges << "RELOAD";
    if (_connection->serverVersionInt() >= 40002) {
        adminGlobalPriveleges << "REPLICATION CLIENT";
        adminGlobalPriveleges << "REPLICATION SLAVE";
    }
    adminGlobalPriveleges << "SHUTDOWN";
    if (_connection->serverVersionInt() >= 40002) {
        adminGlobalPriveleges << "SUPER";
    }


    QStringList globalPrivileges;
    globalPrivileges << readGlobalPriveleges;
    globalPrivileges << writeGlobalPriveleges;
    globalPrivileges << adminGlobalPriveleges;


    _scopePrivileges[UserPrivilege::Scope::Global] = globalPrivileges;

    ////////////////////////////////////////////////////////////////////////////

    QStringList dbLevelPrivileges;
    if (_connection->serverVersionInt() >= 40002) {
        dbLevelPrivileges << "EXECUTE";
    }
    dbLevelPrivileges << "SELECT";
    if (_connection->serverVersionInt() >= 50001) {
        dbLevelPrivileges << "SHOW VIEW";
    }
    dbLevelPrivileges << "ALTER";
    if (_connection->serverVersionInt() >= 50003) {
        dbLevelPrivileges << "ALTER ROUTINE";
    }
    dbLevelPrivileges << "CREATE";
    if (_connection->serverVersionInt() >= 50003) {
        dbLevelPrivileges << "CREATE ROUTINE";
    }
    if (_connection->serverVersionInt() >= 40002) {
        dbLevelPrivileges << "CREATE TEMPORARY TABLES";
    }
    if (_connection->serverVersionInt() >= 50001) {
        dbLevelPrivileges << "CREATE VIEW";
    }
    dbLevelPrivileges << "DELETE";
    dbLevelPrivileges << "DROP";
    if (_connection->serverVersionInt() >= 50106) {
        dbLevelPrivileges << "EVENT";
    }
    dbLevelPrivileges << "INDEX";
    dbLevelPrivileges << "INSERT";
    dbLevelPrivileges << "REFERENCES";
    if (_connection->serverVersionInt() >= 50106) {
        dbLevelPrivileges << "TRIGGER";
    }
    dbLevelPrivileges << "UPDATE";
    dbLevelPrivileges << "GRANT";
    if (_connection->serverVersionInt() >= 40002) {
        dbLevelPrivileges << "LOCK TABLES";
    }

    _scopePrivileges[UserPrivilege::Scope::DatabaseLevel] = dbLevelPrivileges;

    ////////////////////////////////////////////////////////////////////////////

    QStringList tableLevelPrivileges;

    tableLevelPrivileges << "SELECT";
    if (_connection->serverVersionInt() >= 50001) {
        tableLevelPrivileges << "SHOW VIEW";
    }
    tableLevelPrivileges << "ALTER";
    tableLevelPrivileges << "CREATE";
    if (_connection->serverVersionInt() >= 50001) {
        tableLevelPrivileges << "CREATE VIEW";
    }
    tableLevelPrivileges << "DELETE";
    tableLevelPrivileges << "DROP";
    tableLevelPrivileges << "INDEX";
    tableLevelPrivileges << "INSERT";
    tableLevelPrivileges << "REFERENCES";
    if (_connection->serverVersionInt() >= 50106) {
        tableLevelPrivileges << "TRIGGER";
    }
    tableLevelPrivileges << "UPDATE";
    tableLevelPrivileges << "GRANT";

    _scopePrivileges[UserPrivilege::Scope::TableLevel] = tableLevelPrivileges;

    ////////////////////////////////////////////////////////////////////////////

    QStringList routinePrivileges;

    if (_connection->serverVersionInt() >= 40002) {
        routinePrivileges << "EXECUTE";
    }
    if (_connection->serverVersionInt() >= 50003) {
        routinePrivileges << "ALTER ROUTINE";
    }
    routinePrivileges << "GRANT";

    _scopePrivileges[UserPrivilege::Scope::ProcedureLevel] = routinePrivileges;
    _scopePrivileges[UserPrivilege::Scope::FunctionLevel] = routinePrivileges;

    ////////////////////////////////////////////////////////////////////////////

    QStringList tableColumnPrivileges;

    tableColumnPrivileges << "SELECT";
    tableColumnPrivileges << "INSERT";
    tableColumnPrivileges << "REFERENCES";
    tableColumnPrivileges << "UPDATE";

    _scopePrivileges[UserPrivilege::Scope::TableColumnLevel]
            = tableColumnPrivileges;
}

PrivilegeType MySQLUserManager::typeOfPrivilege(
        const QString & privilegeName) const
{
    if (_privilegeTypes.isEmpty()) { // cache
        _privilegeTypes = {
            { "SELECT",         PrivilegeType::Read },
            { "SHOW VIEW",      PrivilegeType::Read },
            { "SHOW DATABASES", PrivilegeType::Read },
            { "PROCESS",        PrivilegeType::Read },
            { "EXECUTE",        PrivilegeType::Read },

            { "ALTER",                    PrivilegeType::Write },
            { "CREATE",                   PrivilegeType::Write },
            { "DROP",                     PrivilegeType::Write },
            { "DELETE",                   PrivilegeType::Write },
            { "UPDATE",                   PrivilegeType::Write },
            { "INSERT",                   PrivilegeType::Write },
            { "ALTER ROUTINE",            PrivilegeType::Write },
            { "CREATE ROUTINE",           PrivilegeType::Write },
            { "CREATE TEMPORARY TABLES",  PrivilegeType::Write },
            { "CREATE VIEW",              PrivilegeType::Write },
            { "INDEX",                    PrivilegeType::Write },
            { "TRIGGER",                  PrivilegeType::Write },
            { "EVENT",                    PrivilegeType::Write },
            { "REFERENCES",               PrivilegeType::Write },
            { "CREATE TABLESPACE",        PrivilegeType::Write },

            { "RELOAD",             PrivilegeType::Admin },
            { "SHUTDOWN",           PrivilegeType::Admin },
            { "REPLICATION CLIENT", PrivilegeType::Admin },
            { "REPLICATION SLAVE",  PrivilegeType::Admin },
            { "SUPER",              PrivilegeType::Admin },
            { "LOCK TABLES",        PrivilegeType::Admin },
            { "GRANT",              PrivilegeType::Admin },
            { "FILE",               PrivilegeType::Admin },
            { "CREATE USER",        PrivilegeType::Admin },
        };
    }

    return _privilegeTypes.value(privilegeName, PrivilegeType::None);
}

void MySQLUserManager::updateUserData(const UserPtr & user,
                                      const UserPtr & userData)
{
    IUserManager::updateUserData(user, userData);
    // clear cached data
    _currentUserName = QString();
    _currentUserNames.clear();
}

void MySQLUserManager::addUser(const UserPtr & user)
{
    _userList.append(user);
    emit userAdded(user);
}

void MySQLUserManager::deleteUser(const UserPtr & user)
{
    _userList.removeOne(user);
    emit userDeleted(user);
}

bool MySQLUserManager::isView(const QString & dbName,
                              const QString & tableName) const
{
    // TODO: something more efficient?

    QString showTableStatusSQL = QString(
                "SHOW TABLE STATUS FROM %1 WHERE `Name` = %2")
                .arg(_connection->quoteIdentifier(dbName))
                .arg(_connection->escapeString(tableName));

    QueryPtr queryResults = _connection->getResults(showTableStatusSQL);

    if (queryResults && !queryResults->isEof()) {

        std::size_t indexOfEngine = queryResults->indexOfColumn("Engine");
        std::size_t indexOfVersion = queryResults->indexOfColumn("Version");

        return queryResults->isNull(indexOfEngine)
                && queryResults->isNull(indexOfVersion);
    }
    return false;
}

} // namespace db
} // namespace meow
