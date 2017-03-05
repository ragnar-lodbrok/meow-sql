#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <memory>
#include <QString>
#include <QStringList>
#include <QMap>
#include "common.h"
#include "connection_parameters.h"
#include "exception.h"
#include "entity/entity_list_for_database.h"

namespace meow {
namespace db {

class Query;
class DataBaseEntitiesFetcher;

typedef std::shared_ptr<Query> QueryPtr;

class Connection
{
public:
    Connection(const ConnectionParameters & params);
    virtual ~Connection();

    bool active() const { return _active; }
    ConnectionParameters * connectionParams() { return &_connectionParams; }
    QString & characterSet() { return _characterSet; }
    bool isUnicode() const { return _isUnicode; }
    unsigned long serverVersionInt() const { return _serverVersionInt; }

    virtual void setCharacterSet(const QString & characterSet);
    void setIsUnicode(bool isUnicode) { _isUnicode = isUnicode; }

    QStringList getColumn(const QString & SQL, std::size_t index = 0); // H: GetCol
    QString getCell(const QString & SQL, std::size_t index = 0); //H:  GetVar
    QueryPtr getResults(const QString & SQL); // H: GetResults(SQL: String): TDBQuery;
    QStringList allDatabases(bool refresh = false);
    EntityListForDataBase * getDbEntities(const QString & dbName, bool refresh = false);

    virtual QStringList fetchDatabases() = 0;
    virtual QueryPtr createQuery() = 0;
    virtual void setActive(bool active) = 0;
    virtual bool ping(bool reconnect) = 0;
    virtual QString getLastError() = 0;
    virtual void doBeforeConnect();
    virtual void doAfterConnect();
    virtual QString fetchCharacterSet() = 0;
    virtual void query(const QString & SQL, bool storeResult = false) = 0; // H: add LogCategory
    virtual std::size_t lastResultsCount() const { return 0; } // H: ResultCount    

    QString quoteIdentifier(const char * identifier, bool alwaysQuote = true, QChar glue = QChar::Null) const;
    QString quoteIdentifier(const QString & identifier, bool alwaysQuote = true, QChar glue = QChar::Null) const;
    virtual QString escapeString(const QString & str, bool processJokerChars = false, bool doQuote = true) const = 0;

protected:
    bool _active;
    db::ulonglong _rowsFound;
    db::ulonglong _rowsAffected;
    int _statementNum; // TODO: why signed, usage?
    QString _serverVersionString;
    unsigned long _serverVersionInt;
    QMap<QString, EntityListForDataBase *> _databaseEntitiesCache; // db name : db's entities

    virtual DataBaseEntitiesFetcher * createDbEntitiesFetcher() = 0;
private:
    //int _connectionStarted;
    //int _serverUptime;
    ConnectionParameters _connectionParams;
    QString _characterSet;
    bool _isUnicode;
    //bool _loginPromptDone;
    //QString _databaseName;
    std::pair<bool, QStringList> _allDatabasesCached; // < cached?, data >
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_H
