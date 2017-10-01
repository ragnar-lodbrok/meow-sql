#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <memory>
#include <QObject>
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
class QueryDataFetcher;
class TableEntity;

typedef std::shared_ptr<Query> QueryPtr;

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(const ConnectionParameters & params);
    virtual ~Connection();

    bool active() const { return _active; }
    ConnectionParameters * connectionParams() { return &_connectionParams; }
    QString & characterSet() { return _characterSet; }
    bool isUnicode() const { return _isUnicode; }
    unsigned long serverVersionInt() const { return _serverVersionInt; }
    QString database() { return _database; }

    virtual void setCharacterSet(const QString & characterSet);
    void setIsUnicode(bool isUnicode) { _isUnicode = isUnicode; }

    QStringList getColumn(const QString & SQL, std::size_t index = 0); // H: GetCol
    QString getCell(const QString & SQL, std::size_t index = 0); //H:  GetVar
    QString getCell(const QString & SQL,const QString & columnName);
    QueryPtr getResults(const QString & SQL); // H: GetResults(SQL: String): TDBQuery;
    QStringList allDatabases(bool refresh = false);
    void setAllDatabases(const QStringList & databases);
    EntityListForDataBase * getDbEntities(const QString & dbName, bool refresh = false);

    QString quoteIdentifier(const char * identifier, bool alwaysQuote = true,
                            QChar glue = QChar::Null) const;
    QString quoteIdentifier(const QString & identifier, bool alwaysQuote = true,
                            QChar glue = QChar::Null) const;

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
    virtual void setDatabase(const QString & database) = 0;
    virtual db::ulonglong getRowCount(const TableEntity * table) = 0;
    virtual QString escapeString(const QString & str,
                                 bool processJokerChars = false,
                                 bool doQuote = true) const = 0;
    virtual QString applyQueryLimit(
            const QString & queryType,
            const QString & queryBody,
            db::ulonglong limit,
            db::ulonglong offset = 0) = 0;

    virtual QueryDataFetcher * createQueryDataFetcher() = 0;
    virtual QString getCreateCode(const Entity * entity) = 0;

    void parseTableStructure(TableEntity * table) const;

    Q_SIGNAL void databaseChanged(const QString & database);

protected:
    bool _active;
    db::ulonglong _rowsFound;
    db::ulonglong _rowsAffected;
    int _statementNum; // TODO: why signed, usage?
    QString _serverVersionString;
    unsigned long _serverVersionInt;
    QMap<QString, EntityListForDataBase *> _databaseEntitiesCache; // db name : db's entities
    QString _database;

    void emitDatabaseChanged(const QString& newName);

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
