#ifndef DB_MYSQLCONNECTION_H
#define DB_MYSQLCONNECTION_H

#include <mysql/mysql.h>
#include "connection.h"

namespace meow {
namespace db {

class Query;

class MySQLConnection : public Connection
{
public:
    MySQLConnection(const ConnectionParameters & params);

    virtual void setActive(bool active) override;

    virtual bool ping(bool reconnect) override;

    virtual QueryPtr createQuery() override;

    virtual QStringList fetchDatabases() override;

    virtual QString getLastError() override;

    virtual QString fetchCharacterSet() override;

    virtual void setCharacterSet(const QString & characterSet) override;
    
    virtual void query(const QString & SQL, bool storeResult = false) override;

private:
    MYSQL * _handle;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQLCONNECTION_H
