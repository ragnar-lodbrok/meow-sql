#include "pg_connection.h"
#include "helpers/logger.h"
#include "pg_query_result.h"
#include "pg_query.h"
#include "db/data_type/pg_connection_data_types.h"

namespace meow {
namespace db {

static const int PG_SEND_QUERY_STATUS_SUCCESS = 1;

PGConnection::PGConnection(const ConnectionParameters & params)
    :Connection(params),
     _handle(nullptr)
{

    _identifierQuote = QLatin1Char('"');

    meowLogDebugC(this) << "Is thread safe: " << PQisthreadsafe();
}

PGConnection::~PGConnection()
{
    meowLogDebugC(this) << "Destroying: " << *connectionParams();
    if (active()) {
        setActive(false);
    }
}

void PGConnection::setActive(bool active)
{
    if (active) {
        doBeforeConnect();

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

            throw db::Exception(error);
        } else {
            _active = true;
            meowLogDebugC(this) << "Connected";

            //_serverVersionString = getCell("SELECT VERSION()");
            //_serverVersionInt = // TODO: parse

            setIsUnicode(true); // TODO?
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

QueryPtr PGConnection::createQuery()
{
    return std::make_shared<PGQuery>(this);
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
    Q_ASSERT(false);
    Q_UNUSED(characterSet);
    // TODO PQsetClientEncoding
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

    int sendQueryStatus = PQsendQuery(_handle, nativeSQL.constData());

    if (sendQueryStatus != PG_SEND_QUERY_STATUS_SUCCESS) {
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query failed: " << error;
        throw db::Exception(error);
    }

    // TODO: H: FWarningCount
    _rowsAffected = 0;
    _rowsFound = 0;

    auto queryResult = std::make_shared<PGQueryResult>(
                PQgetResult(_handle), _handle);

    while (queryResult->nativePtr() != nullptr) {

        ExecStatusType resultStatus = PQresultStatus(queryResult->nativePtr());

        if (resultStatus == PGRES_TUPLES_OK) { // got data

            _rowsFound += static_cast<db::ulonglong>(
                        PQntuples(queryResult->nativePtr()));
            if (storeResult) {
                results << queryResult;
            }

        } else if (resultStatus == PGRES_COMMAND_OK) { // no data but ok

            auto affected =
                QString::fromUtf8( PQcmdTuples(queryResult->nativePtr()) );
            _rowsAffected += static_cast<db::ulonglong>(affected.toInt());

        } else { // something went wrong

            queryResult->clearAll();
            results.clear();
            QString error = getLastError();
            meowLogCC(Log::Category::Error, this) << "Query (next) failed: "
                                                  << error;
            throw db::Exception(error);
        }

        // next query
        queryResult = std::make_shared<PGQueryResult>(
                        PQgetResult(_handle), _handle);
    }

    meowLogDebugC(this) << "Query rows found/affected: " << _rowsFound
                        << "/" << _rowsAffected;

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
        // see https://www.postgresql.org/docs/9.6/ \
        // sql-syntax-lexical.html#SQL-SYNTAX-STRINGS-ESCAPE
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
    Q_ASSERT(false); // TODO
    Q_UNUSED(database);
}

db::ulonglong PGConnection::getRowCount(const TableEntity * table)
{
    Q_ASSERT(false); // TODO
    Q_UNUSED(table);
    return 0;
}

QString PGConnection::applyQueryLimit(
        const QString & queryType,
        const QString & queryBody,
        db::ulonglong limit,
        db::ulonglong offset)
{
    Q_ASSERT(false); // TODO
    return QString();
}

QueryDataFetcher * PGConnection::createQueryDataFetcher()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

CollationFetcher * PGConnection::createCollationFetcher()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString PGConnection::getCreateCode(const Entity * entity)
{
    Q_ASSERT(false); // TODO
    return QString();
}

QStringList PGConnection::tableRowFormats() const
{
    Q_ASSERT(false); // TODO
    return {};
}

bool PGConnection::supportsForeignKeys(const TableEntity * table) const
{
    Q_ASSERT(false); // TODO
    Q_UNUSED(table);
    return true;
}

std::unique_ptr<EntityFilter> PGConnection::entityFilter()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString PGConnection::limitOnePostfix() const
{
    return "LIMIT 1";
}

DataBaseEntitiesFetcher * PGConnection::createDbEntitiesFetcher()
{
    Q_ASSERT(false); // TODO
    return nullptr;
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


} // namespace db
} // namespace meow
