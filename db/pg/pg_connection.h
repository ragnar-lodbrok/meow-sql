#ifndef DB_PG_CONNECTION_H
#define DB_PG_CONNECTION_H

#include "db/connection.h"
#include <libpq-fe.h>
#include "db/entity/entity_filter.h" // TODO: PGEntityFilter

namespace meow {

namespace ssh { class ISSHTunnel; }

namespace db {

class PGConnection : public Connection
{
public:
    explicit PGConnection(const ConnectionParameters & params);

    virtual ~PGConnection() override;

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

protected:
    virtual DataBaseEntitiesFetcher * createDbEntitiesFetcher() override;

    virtual TableEditor * createTableEditor() override;
    virtual DataBaseEditor * createDataBaseEditor() override;

    virtual TableEnginesFetcher * createTableEnginesFetcher() override;

    virtual ConnectionDataTypes * createConnectionDataTypes() override;

    virtual ConnectionFeatures * createFeatures() override;

private:

    QString connectionInfo() const;
    
    QString escapeConnectionParam(const QString & param) const;

    inline QString qu(const char * identifier) const;

    PGconn * _handle;
    std::unique_ptr<ssh::ISSHTunnel> _sshTunnel;
};

} // namespace db
} // namespace meow


#endif // DB_PG_CONNECTION_H
