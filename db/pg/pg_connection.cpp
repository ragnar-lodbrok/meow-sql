#include "pg_connection.h"
#include "pg_connection_query_killer.h"
#include <QElapsedTimer>
#include "helpers/logger.h"
#include "pg_query_result.h"
#include "db/query.h"
#include "pg_query_data_editor.h"
#include "db/data_type/pg_connection_data_types.h"
#include "db/pg/pg_entities_fetcher.h"
#include "pg_query_data_fetcher.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "pg_entity_create_code_generator.h"
#include "ssh/ssh_tunnel_factory.h"

namespace meow {
namespace db {

static const int PG_SEND_QUERY_STATUS_SUCCESS = 1;

PGConnection::PGConnection(const ConnectionParameters & params)
    : Connection(params)
    , _handle(nullptr)
    , _sshTunnel(nullptr)
{

    _identifierQuote = QLatin1Char('"');

    meowLogDebugC(this) << "Is thread safe: " << PQisthreadsafe();
}

PGConnection::~PGConnection()
{
    meowLogDebugC(this) << "Destroying: " << *connectionParams();
    stopThread();
    if (active()) {
        setActive(false);
    }
}

void PGConnection::setActive(bool active)
{
    if (active) {
        doBeforeConnect();

        if (connectionParams()->isSSHTunnel()) {

            ssh::SSHTunnelFactory sshFactory;
            _sshTunnel = sshFactory.createTunnel();
            _sshTunnel->connect(*connectionParams()); // throws an error

            connectionParams()->setHostName("127.0.0.1");
            connectionParams()->setPort(
                        _sshTunnel->params().localPort());
        }

        QString connInfoStr = connectionInfo();
        QByteArray connInfoBytes = connInfoStr.toUtf8();

        meowLogDebugC(this) << "Connecting: " << *connectionParams();

        _handle = PQconnectdb(connInfoBytes.constData());

        if (PQstatus(_handle) == CONNECTION_BAD) {
            QString error = getLastError();
            meowLogCC(Log::Category::Error, this) << "Connect failed: "
                                                  << error;

            PQfinish(_handle);
            _handle = nullptr;

            _sshTunnel.reset();

            throw db::Exception(error);
        } else {
            _active = true;
            meowLogDebugC(this) << "Connected";

            _serverVersionString = getCell("SELECT VERSION()");
            _serverVersionInt = PQserverVersion(_handle);

            setIsUnicode(true);
            setCharacterSet("UTF8");
            meowLogDebugC(this) << "Encoding: " << fetchCharacterSet();

            // H:
            //Query('SET statement_timeout TO '+
            //IntToStr(Parameters.QueryTimeout*1000));

            // H: set database

            doAfterConnect();
        }
    // !active
    } else if (_handle != nullptr) {
        PQfinish(_handle);
        _active = false;
        _handle = nullptr;
        _sshTunnel.reset();
        meowLogDebugC(this) << "Closed";
    }
}

bool PGConnection::ping(bool reconnect)
{
    meowLogDebugC(this) << "Ping";

    if (_active) {
        bool isBroken = (_handle == nullptr);
        if (!isBroken) {
            QString connInfoStr = connectionInfo();
            QByteArray connInfoBytes = connInfoStr.toUtf8();
            isBroken = (PQping(connInfoBytes.constData()) != PQPING_OK);
        }
        if (isBroken) {
            setActive(false); // TODO: why? H: Be sure to release some stuff
                              // before reconnecting
            if (reconnect) {
                setActive(true);
            }
        }
    }

    return _active;
}

QStringList PGConnection::fetchDatabases()
{
    try {
        return getColumn(
                    "SELECT \"nspname\" FROM \"pg_catalog\".\"pg_namespace\" "
                    "ORDER BY \"nspname\"");
    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, this)
            << "Database names not available: " << ex.message();
    }

    return QStringList();
}

QString PGConnection::getLastError()
{
    char * cError = PQerrorMessage(_handle);
    if (cError == nullptr) {
        return QString();
    }
    // a nonempty PQerrorMessage result can consist of multiple lines,
    // and will include a trailing newline.
    size_t errLen = strlen(cError);
    if (errLen > 0) {
        --errLen;
    }
    return QString::fromUtf8(cError, static_cast<int>(errLen));
}

QString PGConnection::fetchCharacterSet()
{
    int encodingId = PQclientEncoding(_handle);
    if (encodingId != -1) {
        const char * encodingName = pg_encoding_to_char(encodingId);
        return QString::fromUtf8(encodingName);
    }
    return QString();
}

void PGConnection::setCharacterSet(const QString & characterSet)
{
    QByteArray charSetBytes = characterSet.toUtf8();
    int result = PQsetClientEncoding(_handle, charSetBytes.constData());
    if (result == -1) {
        meowLogDebugC(this) << "Fail to set encding: " << characterSet;
    }
}

QueryResults PGConnection::query(
        const QString & SQL,
        bool storeResult)
{
    meowLogCC(Log::Category::SQL, this) << SQL;

    ping(true);

    QueryResults results;

    QByteArray nativeSQL;

    if (isUnicode()) {
        nativeSQL = SQL.toUtf8();
    } else {
        nativeSQL = SQL.toLatin1();
    }

    QElapsedTimer elapsedTimer;

    // PQsendQuery is async, so we don't know exec time on server, add all inc
    // waiting PQgetResult to exec time. TODO: get network time somehow?

    int sendQueryStatus = PQsendQuery(_handle, nativeSQL.constData());

    if (sendQueryStatus != PG_SEND_QUERY_STATUS_SUCCESS) {
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query failed: " << error;
        throw db::Exception(error);
    }

    elapsedTimer.start();
    auto queryResult = std::make_shared<PGQueryResult>(this);
    queryResult->init(PQgetResult(_handle), _handle);
    results.incExecDuration(
            std::chrono::milliseconds(elapsedTimer.elapsed()));

    while (queryResult->nativePtr() != nullptr) {

        ExecStatusType resultStatus = PQresultStatus(queryResult->nativePtr());

        if (resultStatus == PGRES_TUPLES_OK) { // got data

            results.incRowsFound(static_cast<db::ulonglong>(
                        PQntuples(queryResult->nativePtr())));

            // TODO: affected is 0 for INSERT ... RETURNING *

            if (storeResult) {
                results << queryResult;
            }

        } else if (resultStatus == PGRES_COMMAND_OK) { // no data but ok

            auto affected =
                QString::fromUtf8( PQcmdTuples(queryResult->nativePtr()) );
            results.incRowsAffected(
                    static_cast<db::ulonglong>(affected.toInt())
            );

        } else { // something went wrong

            queryResult->clearAll();
            results.clear();
            QString error = getLastError();
            meowLogCC(Log::Category::Error, this) << "Query (next) failed: "
                                                  << error;
            throw db::Exception(error);
        }

        // next query
        elapsedTimer.start();
        queryResult = std::make_shared<PGQueryResult>(this);
        queryResult->init(PQgetResult(_handle), _handle);
        results.incExecDuration(
                std::chrono::milliseconds(elapsedTimer.elapsed()));
    }

    meowLogDebugC(this) << "Query rows found/affected: " << results.rowsFound()
                        << "/" << results.rowsAffected();

    return results;
}

QString PGConnection::escapeString(const QString & str,
                             bool processJokerChars,
                             bool doQuote) const
{
    QString res = str;

    if (processJokerChars) {
        Q_ASSERT(false);
        throw std::runtime_error("not implemented");
    }

    res.replace(QLatin1String("\\"), QLatin1String("\\\\")); // keep order
    res.replace(QLatin1Char('\''), QLatin1String("\\'"));

    if (doQuote) {
        /* see https://www.postgresql.org/docs/9.6/ \
         sql-syntax-lexical.html#SQL-SYNTAX-STRINGS-ESCAPE */
        QLatin1Char singleQuote('\'');
        QLatin1String escapeQuote("E'");
        res = escapeQuote + res + singleQuote;
    }

    return res;
}

QString PGConnection::unescapeString(const QString & str) const
{ // TODO: not tested
    QString res = str;

    res.replace(QLatin1String("\\n"), QString(QChar::LineFeed));
    res.replace(QLatin1String("\\r"), QString(QChar::CarriageReturn));
    res.replace(QLatin1String("\\0"), QString(QChar::Null));
    res.replace(QLatin1String("\\t"), QString(QChar::Tabulation));

    res.replace(QLatin1String("\\'"), QChar('\'')); // (\') => (')
    res.replace(QLatin1String("\\\\"), QLatin1String("\\")); // (\\) => (\)

    return res;
}

void PGConnection::setDatabase(const QString & database)
{
    if (database == _database) {
        return;
    }

    // TODO: FOnDatabaseChanged
    // TODO: clear database

    // TODO
    // Always keep public schema in search path, so one can use procedures from it without prefixing
    // See http://www.heidisql.com/forum.php?t=18581#p18905
    //if Value <> 'public' then
    //  s := s + ', ' + EscapeString('public');

    if (!database.isEmpty()) {
        query(QString("SET search_path TO ") + quoteIdentifier(database));
    }
    _database = database;
    emitDatabaseChanged(database);

    // TODO: DetectUSEQuery
    // TODO: SetObjectNamesInSelectedDB
}

db::ulonglong PGConnection::getRowCount(const TableEntity * table)
{

    const QString SQL = "SELECT " + qu("reltuples") + "::bigint FROM "
    + qu("pg_class") + " LEFT JOIN " + qu("pg_namespace")
    + " ON (" + qu("pg_namespace") + '.' + qu("oid") + " = "
    + qu("pg_class") + '.' + qu("relnamespace") + ")" + " WHERE "
    + qu("pg_class") + '.' + qu("relkind") + " = " + escapeString("r") +
    + " AND " + qu("pg_namespace") + '.' + qu("nspname")
    + " = " + escapeString(table->database()->name())
    + " AND " + qu("pg_class") + '.' + qu("relname")
    + " = " + escapeString(table->name());

    return getCell(SQL).toULongLong();

}

QString PGConnection::applyQueryLimit(
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

QueryDataFetcher * PGConnection::createQueryDataFetcher()
{
    return new PGQueryDataFetcher(this);
}

CollationFetcher * PGConnection::createCollationFetcher()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString PGConnection::getCreateCode(const Entity * entity)
{
    PGEntityCreateCodeGenerator generator(this);
    return generator.run(entity);
}

QStringList PGConnection::tableRowFormats() const
{
    Q_ASSERT(false); // TODO
    return {};
}

std::unique_ptr<EntityFilter> PGConnection::entityFilter()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

std::shared_ptr<QueryDataEditor> PGConnection::queryDataEditor()
{
    return std::make_shared<PGQueryDataEditor>();
}

QString PGConnection::limitOnePostfix(bool select) const
{
    if (select) {
        return "LIMIT 1";
    } else {
        return ""; // not so simple for PG
    }
}

int64_t PGConnection::connectionIdOnServer()
{
    // TODO: not tested
    if (_connectionIdOnServer == -1) {
        if (ping(false)) {
            _connectionIdOnServer = PQbackendPID(_handle);
        }
    }
    return _connectionIdOnServer;
}

ConnectionQueryKillerPtr PGConnection::createQueryKiller() const
{
    return std::make_shared<PGConnectionQueryKiller>(
                const_cast<PGConnection *>(this));
}

DataBaseEntitiesFetcher * PGConnection::createDbEntitiesFetcher()
{
    return new PGEntitiesFetcher(this);
}

TableEditor * PGConnection::createTableEditor()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

DataBaseEditor * PGConnection::createDataBaseEditor()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

TableEnginesFetcher * PGConnection::createTableEnginesFetcher()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString PGConnection::connectionInfo() const
{
    QString dbName = connectionParams()->databases();
    if (dbName.isEmpty()) {
        dbName = QString("postgres");
    }

    QString port = QString::number(connectionParams()->port());

    QString connInfoStr = QString(
        "host=%1 "
        "port=%2 "
        "user=%3 "
        "password=%4 "
        "dbname=%5 "
        "application_name=%6")
     .arg(escapeConnectionParam(connectionParams()->hostName()))
     .arg(escapeConnectionParam(port))
     .arg(escapeConnectionParam(connectionParams()->userName()))
     .arg(escapeConnectionParam(connectionParams()->password()))
     .arg(escapeConnectionParam(dbName))
     .arg(escapeConnectionParam("meow-sql")); // TODO: const

    return connInfoStr;
}

QString PGConnection::escapeConnectionParam(const QString & param) const
{

    // Single quotes and backslashes within the value must be escaped with a
    // backslash, i.e., \' and \\.
    QString res = param;

    res.replace(QLatin1String("\\"), QLatin1String("\\\\")); // keep order
    res.replace(QLatin1Char('\''), QLatin1String("\\'"));

    QLatin1Char singleQuote('\'');
    res = singleQuote + res + singleQuote;

    return res;
}

ConnectionDataTypes * PGConnection::createConnectionDataTypes()
{
    return new PGConnectionDataTypes(this);
}

ConnectionFeatures * PGConnection::createFeatures()
{
    return new PGConnectionFeatures(this);
}

QString PGConnection::qu(const char * identifier) const
{
    return this->quoteIdentifier(identifier);
}

} // namespace db
} // namespace meow
