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

    virtual ~MySQLConnection();

    virtual void setActive(bool active) override;

    virtual bool ping(bool reconnect) override;

    virtual QueryPtr createQuery() override;

    virtual QStringList fetchDatabases() override;

    virtual QString getLastError() override;

    virtual QString fetchCharacterSet() override;

    virtual void setCharacterSet(const QString & characterSet) override;
    
    virtual void query(const QString & SQL, bool storeResult = false) override;

    virtual std::size_t lastResultsCount() const override;

    virtual QString escapeString(const QString & str, bool processJokerChars = false, bool doQuote = true) const override;

    virtual void setDatabase(const QString & database) override;

    virtual db::ulonglong getRowCount(const TableEntity * table) override;

    virtual QString applyQueryLimit(
            const QString & queryType,
            const QString & queryBody,
            db::ulonglong limit,
            db::ulonglong offset = 0) override;

    virtual QueryDataFetcher * createQueryDataFetcher() override;

    virtual QString getCreateCode(const Entity * entity) override;

    MySQLResult lastRawResultAt(std::size_t index) const;

protected:
    virtual DataBaseEntitiesFetcher * createDbEntitiesFetcher() override;

    virtual TableEditor * createTableEditor() override;

private:
    MYSQL * _handle;
    std::vector<MySQLResult> _lastRawResults;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQLCONNECTION_H
