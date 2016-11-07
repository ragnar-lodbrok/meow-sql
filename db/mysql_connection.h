#ifndef DB_MYSQLCONNECTION_H
#define DB_MYSQLCONNECTION_H

#include <vector>
#include <mysql/mysql.h>
#include "connection.h"

namespace meow {
namespace db {

typedef std::shared_ptr<MYSQL_RES> MySQLResult; // calls mysql_free_result in deleter

MySQLResult createSharedMySQLResultFromNative(MYSQL_RES * nativeMySQLRes);

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

    virtual std::size_t lastResultsCount() const override;

    MySQLResult lastRawResultAt(std::size_t index) const;

private:
    MYSQL * _handle;
    std::vector<MySQLResult> _lastRawResults;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQLCONNECTION_H
