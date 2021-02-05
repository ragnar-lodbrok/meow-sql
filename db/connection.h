#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <memory>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include "common.h"
#include "native_query_result_interface.h"
#include "connection_parameters.h"
#include "exception.h"
#include "entity/entity_list_for_database.h"
#include "table_structure_parser.h"
#include "view_structure_parser.h"
#include "routine_structure_parser.h"
#include "collation_fetcher.h"
#include "db/data_type/connection_data_types.h"
#include "connection_features.h"
#include "session_variables.h"
#include "user_manager.h"

namespace meow {
namespace db {

class Query;
class DataBaseEntitiesFetcher;
class QueryDataFetcher;
class TableEntity;
class EntityInDatabase;
class DataBaseEntity;
class TableEditor;
class ViewEditor;
class RoutineEditor;
class DataBaseEditor;
class TableEnginesFetcher;
class EntityFilter;
class QueryDataEditor;
class ViewEntity;
class RoutineEntity;
class TriggerEntity;

typedef std::shared_ptr<Query> QueryPtr;

class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(const ConnectionParameters & params);
    virtual ~Connection();

    bool active() const { return _active; }
    ConnectionParameters * connectionParams() { return &_connectionParams; }
    const ConnectionParameters * connectionParams() const {
        return &_connectionParams; }
    QString & characterSet() { return _characterSet; }
    bool isUnicode() const { return _isUnicode; }
    int serverVersionInt() const { return _serverVersionInt; }
    QString database() const { return _database; }

    virtual void setCharacterSet(const QString & characterSet);
    void setIsUnicode(bool isUnicode) { _isUnicode = isUnicode; }

    QStringList getColumn(const QString & SQL,
                          std::size_t index = 0); // H: GetCol
    QString getCell(const QString & SQL,
                    std::size_t index = 0); //H:  GetVar
    QString getCell(const QString & SQL, const QString & columnName);
    QStringList getRow(const QString & SQL);
    QList<QStringList> getRows(const QString & SQL);
    QueryPtr getResults(const QString & SQL); // H: GetResults(SQL: String):
    QStringList allDatabases(bool refresh = false);
    void setAllDatabases(const QStringList & databases);
    void setUseAllDatabases();
    EntityListForDataBase * getDbEntities(const QString & dbName,
                                          bool refresh = false);
    bool deleteAllCachedEntitiesInDatabase(const QString & dbName);

    QString quoteIdentifier(const char * identifier,
                                   bool alwaysQuote = true,
                                   QChar glue = QChar::Null) const;
    QString quoteIdentifier(const QString & identifier,
                                   bool alwaysQuote = true,
                                   QChar glue = QChar::Null) const;
    QStringList quoteIdentifiers(const QStringList & identifiers) const;
    QString dequoteIdentifier(const QString & identifier,
                              QChar glue = QChar::Null) const;

    const QStringList collationList();
    QString serverDefaultCollation();
    QString serverPrefferedCollation();

    const QStringList tableEnginesList();
    QString defaultTableEngine();

    virtual QStringList fetchDatabases() = 0;
    virtual QueryPtr createQuery() = 0;
    virtual void setActive(bool active) = 0;
    virtual bool ping(bool reconnect) = 0;
    virtual QString getLastError() = 0;
    virtual void doBeforeConnect();
    virtual void doAfterConnect();
    virtual QString fetchCharacterSet() = 0;
    virtual QueryResults query(
            const QString & SQL,
            bool storeResult = false) = 0; // H: add LogCategory
    virtual void setDatabase(const QString & database) = 0;
    virtual db::ulonglong getRowCount(const TableEntity * table) = 0;
    virtual QString escapeString(const QString & str,
                                 bool processJokerChars = false,
                                 bool doQuote = true) const = 0;
    virtual QString unescapeString(const QString & str) const = 0;
    virtual QString applyQueryLimit(
            const QString & queryType,
            const QString & queryBody,
            db::ulonglong limit,
            db::ulonglong offset = 0) = 0;

    virtual QueryDataFetcher * createQueryDataFetcher() = 0; // TODO return as unique_ptr
    virtual QString getCreateCode(const Entity * entity) = 0;
    virtual QStringList tableRowFormats() const = 0;
    virtual std::unique_ptr<EntityFilter> entityFilter() = 0;
    virtual std::shared_ptr<QueryDataEditor> queryDataEditor(); // TODO = 0 ?
    virtual QString limitOnePostfix(bool select) const;
    virtual QDateTime currentServerTimestamp();

    void parseTableStructure(TableEntity * table, bool refresh = false);
    void parseViewStructure(ViewEntity * view, bool refresh = false);
    void parseRoutineStructure(RoutineEntity * routine, bool refresh = false);
    void parseTriggerStructure(TriggerEntity * trigger, bool refresh = false);

    bool editEntityInDB(EntityInDatabase * entity, EntityInDatabase * newData);
    bool insertEntityToDB(EntityInDatabase * entity);
    bool dropEntityInDB(EntityInDatabase * entity);
    bool dropDatabase(DataBaseEntity * database);
    void createDatabase(const QString & name,
                        const QString & collation = QString());
    bool editDatabase(DataBaseEntity * database,
                      const QString & newName,
                      const QString & newCollation = QString());

    ConnectionDataTypes * dataTypes();

    ConnectionFeatures * features();

    SessionVariables * variables();

    IUserManager * userManager();

    // TODO: rename to activeDatabaseChanged
    Q_SIGNAL void databaseChanged(const QString & database);

    QLatin1Char getIdentQuote() const { return _identifierQuote; }

protected:
    bool _active;
    db::ulonglong _rowsFound; // TODO: rm?
    db::ulonglong _rowsAffected; // TODO: rm?
    QString _serverVersionString;
    int _serverVersionInt;

    // db name : db's entities
    QMap<QString, EntityListForDataBase *> _databaseEntitiesCache;
    QString _database;
    QLatin1Char _identifierQuote;

    void emitDatabaseChanged(const QString& newName);

    virtual DataBaseEntitiesFetcher * createDbEntitiesFetcher() = 0;
    // TODO: move editors and edit methods to separate class
    virtual TableEditor * createTableEditor() = 0;
    virtual ViewEditor * createViewEditor();
    virtual RoutineEditor * createRoutineEditor();
    virtual DataBaseEditor * createDataBaseEditor() = 0;
    virtual CollationFetcher * createCollationFetcher() = 0;
    virtual TableEnginesFetcher * createTableEnginesFetcher() = 0;
    virtual ConnectionDataTypes * createConnectionDataTypes() = 0;
    virtual ConnectionFeatures * createFeatures();
    virtual ITableStructureParser * createTableStructureParser();
    virtual SessionVariables * createVariables() { return nullptr; }
    virtual IUserManager * createUserManager() { return nullptr; }

private:
    //int _connectionStarted;
    //int _serverUptime;
    ConnectionParameters _connectionParams;
    QString _characterSet;
    bool _isUnicode;
    //bool _loginPromptDone;
    //QString _databaseName;
    std::pair<bool, QStringList> _allDatabasesCached; // < cached?, data >
    std::unique_ptr<ITableStructureParser> _tableStructureParser;
    std::unique_ptr<ViewStructureParser> _viewStructureParser;
    std::unique_ptr<RoutineStructureParser> _routineStructureParser;
    std::unique_ptr<CollationFetcher> _collationFetcher;
    std::unique_ptr<TableEnginesFetcher> _tableEnginesFetcher;
    std::shared_ptr<ConnectionDataTypes> _dataTypes;
    std::shared_ptr<ConnectionFeatures> _features;
    std::unique_ptr<SessionVariables> _variables;
    std::unique_ptr<IUserManager> _userManager;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_H
