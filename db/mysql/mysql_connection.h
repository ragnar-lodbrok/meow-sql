#ifndef DB_MYSQLCONNECTION_H
#define DB_MYSQLCONNECTION_H

#include <vector>
#include <mysql/mysql.h>
#include "db/connection.h"

namespace meow {

namespace ssh { class ISSHTunnel; }

namespace db {

class MySQLConnection : public Connection
{
public:
    explicit MySQLConnection(const ConnectionParameters & params);

    virtual ~MySQLConnection() override;

    virtual void setActive(bool active) override;

    virtual bool ping(bool reconnect) override;

    virtual QueryPtr createQuery() override;

    virtual QStringList fetchDatabases() override;

    virtual QString getLastError() override;

    virtual QString fetchCharacterSet() override;

    virtual void setCharacterSet(const QString & characterSet) override;
    
    virtual QueryResults query(
            const QString & SQL,
            bool storeResult = false) override;

    virtual QString escapeString(const QString & str,
                                 bool processJokerChars = false,
                                 bool doQuote = true) const override;

    virtual QString unescapeString(const QString & str) const override;

    virtual void setDatabase(const QString & database) override;

    virtual db::ulonglong getRowCount(const TableEntity * table) override;

    virtual QString applyQueryLimit(
            const QString & queryType,
            const QString & queryBody,
            db::ulonglong limit,
            db::ulonglong offset = 0) override;

    virtual QueryDataFetcher * createQueryDataFetcher() override;

    virtual CollationFetcher * createCollationFetcher() override;

    virtual QString getCreateCode(const Entity * entity) override;

    virtual QStringList tableRowFormats() const override;

    virtual std::unique_ptr<EntityFilter> entityFilter() override;

    virtual std::shared_ptr<QueryDataEditor> queryDataEditor() override;

    virtual QString limitOnePostfix(bool select) const override;

    virtual int64_t connectionIdOnServer() override;

protected:
    virtual DataBaseEntitiesFetcher * createDbEntitiesFetcher() override;

    virtual TableEditor * createTableEditor() override;
    virtual DataBaseEditor * createDataBaseEditor() override;

    virtual TableEnginesFetcher * createTableEnginesFetcher() override;
    virtual ConnectionDataTypes * createConnectionDataTypes() override;

    virtual ConnectionFeatures * createFeatures() override;   
    virtual SessionVariables * createVariables() override;
    virtual IUserManager * createUserManager() override;
    virtual IUserEditor * createUserEditor() override;

private:

    QString getViewCreateCode(const ViewEntity * view);

    MYSQL * _handle;
    std::unique_ptr<ssh::ISSHTunnel> _sshTunnel;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQLCONNECTION_H
