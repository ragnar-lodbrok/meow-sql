#include "pg_connection.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

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
        PQstatus(_handle);
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
    Q_ASSERT(false); // TODO
    return nullptr;
}

QStringList PGConnection::fetchDatabases()
{
    Q_ASSERT(false); // TODO
    return {};
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
    Q_ASSERT(false); // TODO
    Q_UNUSED(SQL);
    Q_UNUSED(storeResult);
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
{
    Q_ASSERT(false); // TODO
    Q_UNUSED(str);
    return QString();
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
    return true;
}

std::unique_ptr<EntityFilter> PGConnection::entityFilter()
{
    Q_ASSERT(false); // TODO
    return nullptr;
}

QString PGConnection::limitOnePostfix() const
{
    Q_ASSERT(false); // TODO
    return QString();
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

} // namespace db
} // namespace meow
