#include <QDebug>
#include "mysql_entities_fetcher.h"
#include "db/mysql_connection.h"
#include "db/query.h"
#include "table_entity.h"
#include "view_entity.h"
#include "function_entity.h"
#include "procedure_entity.h"
#include "trigger_entity.h"

namespace meow {
namespace db {

MySQLEntitiesFetcher::MySQLEntitiesFetcher(MySQLConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void MySQLEntitiesFetcher::run(const QString & dbName, EntityListForDataBase * toList) // override
{
    // TODO SELECT DEFAULT_COLLATION_NAME

    fetchTablesViews(dbName, toList);

    unsigned long serverVersion = _connection->serverVersionInt();

    if (serverVersion >= 50000) {
        fetchStoredFunctions(dbName, toList);
        fetchStoredProcedures(dbName, toList);
    }

    if (serverVersion >= 50010) {
        fetchTriggers(dbName, toList);
        // TODO: Events
    }
}

void MySQLEntitiesFetcher::fetchTablesViews(const QString & dbName,
                                            EntityListForDataBase * toList)
{
    bool fullTableStatus = _connection->connectionParams()->fullTableStatus();

    QueryPtr queryResults;

    if (fullTableStatus || (QString::compare(dbName, "INFORMATION_SCHEMA", Qt::CaseInsensitive) == 0)) {

        try {
            queryResults = _connection->getResults(QString("SHOW TABLE STATUS FROM ") + _connection->quoteIdentifier(dbName));
        } catch(meow::db::Exception & ex) {
            qDebug() << "[MySQLConnection] " << "Failed to fetch tables/views: " << ex.message();
            return;
        }
    } else {
        qDebug() << "Not implemented";
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName     = resPtr->indexOfColumn("Name");
        std::size_t indexOfEngine   = resPtr->indexOfColumn("Engine");
        std::size_t indexOfVersion  = resPtr->indexOfColumn("Version");
        std::size_t indexOfDataLen  = resPtr->indexOfColumn("Data_length");
        std::size_t indexOfIndexLen = resPtr->indexOfColumn("Index_length");

        while (resPtr->isEof() == false) {

            bool isView = resPtr->isNull(indexOfEngine) && resPtr->isNull(indexOfVersion);

            QString name = resPtr->curRowColumn(indexOfName);

            if (isView) {
                ViewEntity * view = new ViewEntity(name);
                toList->list()->append(view);
            } else {
                TableEntity * table = new TableEntity(name);

                if (!resPtr->isNull(indexOfDataLen) && !resPtr->isNull(indexOfIndexLen)) {
                    auto dataLen = resPtr->curRowColumn(indexOfDataLen).toULongLong();
                    auto indexLen = resPtr->curRowColumn(indexOfIndexLen).toULongLong();
                    table->setDataSize(dataLen + indexLen);
                }

                toList->list()->append(table);
            }

            resPtr->seekNext();
        }
    }
}

void MySQLEntitiesFetcher::fetchStoredFunctions(const QString & dbName,
                                                EntityListForDataBase * toList)
{
    QueryPtr queryResults;

    try {
        queryResults = _connection->getResults(QString("SHOW FUNCTION STATUS WHERE `Db` = ") + _connection->escapeString(dbName));
    } catch(meow::db::Exception & ex) {
        qDebug() << "[MySQLConnection] " << "Failed to fetch stored functions: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName = resPtr->indexOfColumn("Name");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfName);

            FunctionEntity * func = new FunctionEntity(name);
            toList->list()->append(func);

            resPtr->seekNext();
        }
    }
}

void MySQLEntitiesFetcher::fetchStoredProcedures(const QString & dbName,
                                                 EntityListForDataBase * toList)
{
    QueryPtr queryResults;

    try {
        queryResults = _connection->getResults(QString("SHOW PROCEDURE STATUS WHERE `Db` = ") + _connection->escapeString(dbName));
    } catch(meow::db::Exception & ex) {
        qDebug() << "[MySQLConnection] " << "Failed to fetch stored procedures: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName = resPtr->indexOfColumn("Name");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfName);

            ProcedureEntity * proc = new ProcedureEntity(name);
            toList->list()->append(proc);

            resPtr->seekNext();
        }
    }
}

void MySQLEntitiesFetcher::fetchTriggers(const QString & dbName,
                                         EntityListForDataBase * toList)
{

    QueryPtr queryResults;

    try {
        queryResults = _connection->getResults(QString("SHOW TRIGGERS FROM ") + _connection->quoteIdentifier(dbName));
    } catch(meow::db::Exception & ex) {
        qDebug() << "[MySQLConnection] " << "Failed to fetch triggers: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfTrigger = resPtr->indexOfColumn("Trigger");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfTrigger);

            TriggerEntity * trigger = new TriggerEntity(name);
            toList->list()->append(trigger);

            resPtr->seekNext();
        }
    }

}

} // namespace db
} // namespace meow

