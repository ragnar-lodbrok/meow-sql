#include "sqlite_connection.h"
#include "db/qtsql/qtsql_query.h"
#include "helpers/logger.h"
#include "sqlite_entities_fetcher.h"
#include "db/data_type/sqlite_connection_datatypes.h"
#include "db/query_data_fetcher.h"
#include "db/entity/entity_list_for_database.h"
#include "db/entity/table_entity.h"
#include "sqlite_table_structure_parser.h"

// https://doc.qt.io/qt-5/sql-programming.html
// https://code.qt.io/cgit/qt/qtbase.git/tree/examples/sql/sqlbrowser/browser.cpp?h=5.14


namespace meow {
namespace db {

SQLiteConnection::SQLiteConnection(const ConnectionParameters & params)
    :Connection(params)
{
    // Listening: Stormlord - Leviathan

    // TODO: Qt SQL has multithreading limitations

    _handle = QSqlDatabase::addDatabase("QSQLITE", params.sessionName());
}

SQLiteConnection::~SQLiteConnection()
{
    meowLogDebugC(this) << "Destroying: " << *connectionParams();

    if (active()) {

        try {
            this->query("PRAGMA optimize");
        } catch (db::Exception & exc) {
            meowLogCC(Log::Category::Error, this) << "PRAGMA optimize failed: "
                                                  << exc.message();
        }

        setActive(false);
    }

    // https://doc.qt.io/qt-5/qsqldatabase.html#removeDatabase
    _handle = QSqlDatabase();

    QSqlDatabase::removeDatabase(connectionParams()->sessionName());
}

void SQLiteConnection::setActive(bool active)
{
    if (active) {
        doBeforeConnect();

        _handle.setDatabaseName(connectionParams()->fileName());

        meowLogDebugC(this) << "Connecting: " << *connectionParams();

        if (_handle.open()) {
            _active = true;
            meowLogDebugC(this) << "Connected";

            setIsUnicode(true);

            doAfterConnect();
        } else {
            QString error = getLastError();
            meowLogCC(Log::Category::Error, this) << "Connect failed: "
                                                  << error;
            throw db::Exception(error);
        }
    } else {     // !active
        _handle.close();
        _active = false;
        meowLogDebugC(this) << "Closed";
    }
}

bool SQLiteConnection::ping(bool reconnect)
{
    Q_UNUSED(reconnect)
    return true;
}

QueryPtr SQLiteConnection::createQuery()
{
    return std::make_shared<QtSQLQuery>(this);
}

QStringList SQLiteConnection::fetchDatabases()
{
    return QStringList("main");
}

QString SQLiteConnection::getLastError()
{
    return _handle.lastError().text();
}

QString SQLiteConnection::fetchCharacterSet()
{
    return QString::null;
}

void SQLiteConnection::setCharacterSet(const QString & characterSet)
{
    Q_UNUSED(characterSet);
}

QueryResults SQLiteConnection::query(
        const QString & SQL,
        bool storeResult)
{
    meowLogCC(Log::Category::SQL, this) << SQL;

    // Some DBs like SQLite can't execute multiple sentences at once
    QStringList SQLs = SQL.split(";", QString::SkipEmptyParts);

    _rowsAffected = 0;
    _rowsFound = 0;

    QueryResults results;

    for (const QString & SQL : SQLs) {

        QSqlQuery * query = new QSqlQuery(_handle);

        auto queryResult = std::make_shared<QtSQLQueryResult>(query, &_handle);

        if (query->exec(SQL) == false) {
            QString error = query->lastError().text();
            meowLogCC(Log::Category::Error, this) << "Query failed: " << error;
            throw db::Exception(error);
        }

        _rowsAffected += query->numRowsAffected();
        _rowsFound += queryResult->rowsCount();

        if (storeResult && queryResult->rowsCount() > 0) {
            results << queryResult;
        }
    }

    meowLogDebugC(this) << "Query rows found/affected: " << _rowsFound
                        << "/" << _rowsAffected;

    return results;
}

QString SQLiteConnection::escapeString(const QString & str,
                             bool processJokerChars,
                             bool doQuote) const
{
    QString res = str;

    if (processJokerChars) {
        Q_ASSERT(false);
        throw std::runtime_error("not implemented");
    }

    // A string constant is formed by enclosing the string in single quotes
    // ('). A single quote within the string can be encoded by putting
    // two single quotes in a row - as in Pascal. C-style escapes using
    // the backslash character are not supported because they are not standard
    // SQL.

    res.replace(QLatin1Char('\''), QLatin1String("''")); // (') -> ('')

    if (doQuote) {
        QLatin1Char singleQuote('\'');
        res = singleQuote + res + singleQuote;
    }

    return res;
}

QString SQLiteConnection::unescapeString(const QString & str) const
{
    QString res = str;

    // TODO: need below?
    //res.replace(QLatin1String("\\n"), QString(QChar::LineFeed));
    //res.replace(QLatin1String("\\r"), QString(QChar::CarriageReturn));
    //res.replace(QLatin1String("\\0"), QString(QChar::Null));
    //res.replace(QLatin1String("\\t"), QString(QChar::Tabulation));

    res.replace(QLatin1String("''"), QChar('\'')); // ('') -> (')

    return res;
}

void SQLiteConnection::setDatabase(const QString & database)
{
    if (database == _database) {
        return;
    }

    _database = database;
    emitDatabaseChanged(database);
}

db::ulonglong SQLiteConnection::getRowCount(const TableEntity * table)
{
    if (!getDbEntities(database())->hasEntity("sqlite_stat1")) {
        try {
            // create "sqlite_stat1"
            this->query("ANALYZE"); // TODO: pass db name only
        } catch (db::Exception & exc) {
            meowLogCC(Log::Category::Error, this) << "ANALYZE failed: "
                                                  << exc.message();
        }
    }

    try {
        // TODO: show 'approx' near rows count got this way

        QString SQL = QString("SELECT `stat` FROM `sqlite_stat1` " \
                              "WHERE `tbl` = %1 LIMIT 1")
                .arg(escapeString(table->name()));
        QString stat = this->getCell(SQL);
        if (stat.isEmpty() == false) {
            return stat.split(QChar(' '))[0].toULongLong();
        }
    } catch (db::Exception & exc) {
        meowLogCC(Log::Category::Error, this)
                << "Querying `sqlite_stat1` failed: "
                << exc.message();
    }

    // TODO: last resort
    // SELECT MAX(_ROWID_) FROM "table" LIMIT 1; ?

    return 0;
}

QString SQLiteConnection::applyQueryLimit(
        const QString & queryType,
        const QString & queryBody,
        db::ulonglong limit,
        db::ulonglong offset)
{
    QString res = queryType + " " + queryBody
        + " LIMIT " + QString::number(limit);
    if (offset > 0) {
        res += " OFFSET " + QString::number(offset);
    }

    return res;
}

QueryDataFetcher * SQLiteConnection::createQueryDataFetcher()
{
    return new QueryDataFetcher(this);
}

CollationFetcher * SQLiteConnection::createCollationFetcher()
{
    //Q_ASSERT(false); // TODO
    return nullptr;
}

QString SQLiteConnection::getCreateCode(const Entity * entity)
{
    QString typeStr;

    switch (entity->type()) {
    case Entity::Type::Table:
        typeStr = QString("table");
        break;
    case Entity::Type::View:
        typeStr = QString("view");
        break;
    default:
        meowLogDebugC(this) << "Unimplemented type in " << __FUNCTION__;
        Q_ASSERT(false);
        return QString();
    }

    QString SQL = QString("SELECT `sql` FROM `sqlite_master`" \
                          " where name = %1 and type = %2")
            .arg(escapeString(entity->name()))
            .arg(escapeString(typeStr));

    return getCell(SQL, 0);
}

QStringList SQLiteConnection::tableRowFormats() const
{
    //Q_ASSERT(false); // TODO
    return {};
}

std::unique_ptr<EntityFilter> SQLiteConnection::entityFilter()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

std::shared_ptr<QueryDataEditor> SQLiteConnection::queryDataEditor()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString SQLiteConnection::limitOnePostfix(bool select) const
{
    if (select) {
        return "LIMIT 1";
    } else {
        return ""; // UPDATE may not work
    }
}

DataBaseEntitiesFetcher * SQLiteConnection::createDbEntitiesFetcher()
{
    return new SQLiteEntitiesFetcher(this);
}

TableEditor * SQLiteConnection::createTableEditor()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

DataBaseEditor * SQLiteConnection::createDataBaseEditor()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

TableEnginesFetcher * SQLiteConnection::createTableEnginesFetcher()
{
    //Q_ASSERT(false); // TODO
    return nullptr;
}

ConnectionDataTypes * SQLiteConnection::createConnectionDataTypes()
{
    return new SQLiteConnectionDataTypes(this);
}

ConnectionFeatures * SQLiteConnection::createFeatures()
{
    return new QtSQLiteConnectionFeatures(this);
}

ITableStructureParser * SQLiteConnection::createTableStructureParser()
{
    return new SQLiteTableStructureParser(this);
}

} // namespace db
} // namespace meow
