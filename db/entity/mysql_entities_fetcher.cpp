#include "mysql_entities_fetcher.h"
#include "db/mysql_connection.h"
#include "db/query.h"
#include "table_entity.h"
#include "view_entity.h"
#include "function_entity.h"
#include "procedure_entity.h"
#include "trigger_entity.h"
#include "helpers/parsing.h"
#include "helpers/logger.h"

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
            meowLogCC(Log::Category::Error, _connection)
                    << "Failed to fetch tables/views: " << ex.message();
            return;
        }
    } else {
        meowLogDebug() << "Not implemented";
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName       = resPtr->indexOfColumn("Name");
        std::size_t indexOfEngine     = resPtr->indexOfColumn("Engine");
        std::size_t indexOfDataLen    = resPtr->indexOfColumn("Data_length");
        std::size_t indexOfIndexLen   = resPtr->indexOfColumn("Index_length");
        std::size_t indexOfRows       = resPtr->indexOfColumn("Rows");
        std::size_t indexOfCollation  = resPtr->indexOfColumn("Collation");
        std::size_t indexOfCreateTime = resPtr->indexOfColumn("Create_time");
        std::size_t indexOfUpdateTime = resPtr->indexOfColumn("Update_time");
        std::size_t indexOfVersion    = resPtr->indexOfColumn("Version");

        while (resPtr->isEof() == false) {

            bool isView = resPtr->isNull(indexOfEngine) && resPtr->isNull(indexOfVersion);

            QString name = resPtr->curRowColumn(indexOfName);

            if (isView) {
                ViewEntity * view = new ViewEntity(name);
                toList->list()->append(view);
            } else {
                TableEntity * table = new TableEntity(name);
                // data size
                if (!resPtr->isNull(indexOfDataLen) && !resPtr->isNull(indexOfIndexLen)) {
                    auto dataLen = resPtr->curRowColumn(indexOfDataLen).toULongLong();
                    auto indexLen = resPtr->curRowColumn(indexOfIndexLen).toULongLong();
                    table->setDataSize(dataLen + indexLen);
                }
                // engine
                table->setEngine(resPtr->curRowColumn(indexOfEngine));
                // rows count
                if (!resPtr->isNull(indexOfRows)) {
                    table->setRowsCount(
                        resPtr->curRowColumn(indexOfRows).toULongLong()
                    );
                }
                // collation
                if (!resPtr->isNull(indexOfCollation)) {
                    table->setCollation(
                        resPtr->curRowColumn(indexOfCollation)
                    );
                }
                // create time
                if (!resPtr->isNull(indexOfCreateTime)) {
                    table->setCreated(
                        helpers::parseDateTime(
                            resPtr->curRowColumn(indexOfCreateTime)
                        )
                    );
                }
                // update time
                if (!resPtr->isNull(indexOfUpdateTime)) {
                    table->setUpdated(
                        helpers::parseDateTime(
                            resPtr->curRowColumn(indexOfUpdateTime)
                        )
                    );
                }
                // version
                if (!resPtr->isNull(indexOfVersion)) {
                    table->setVersion(
                        resPtr->curRowColumn(indexOfVersion).toULongLong()
                    );
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
        queryResults = _connection->getResults(
            QString("SHOW FUNCTION STATUS WHERE `Db` = ")
                    + _connection->escapeString(dbName));
    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
                << "Failed to fetch stored functions: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName     = resPtr->indexOfColumn("Name");
        std::size_t indexOfCreated  = resPtr->indexOfColumn("Created");
        std::size_t indexOfModified = resPtr->indexOfColumn("Modified");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfName);

            FunctionEntity * func = new FunctionEntity(name);

            if (!resPtr->isNull(indexOfCreated)) {
                func->setCreated(
                    helpers::parseDateTime(
                        resPtr->curRowColumn(indexOfCreated)
                    )
                );
            }

            if (!resPtr->isNull(indexOfModified)) {
                func->setUpdated(
                    helpers::parseDateTime(
                        resPtr->curRowColumn(indexOfModified)
                    )
                );
            }

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
        queryResults = _connection->getResults(
            QString("SHOW PROCEDURE STATUS WHERE `Db` = ")
                    + _connection->escapeString(dbName));
    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
                << "Failed to fetch stored procedures: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfName    = resPtr->indexOfColumn("Name");
        std::size_t indexOfCreated = resPtr->indexOfColumn("Created");
        std::size_t indexOfModified = resPtr->indexOfColumn("Modified");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfName);

            ProcedureEntity * proc = new ProcedureEntity(name);

            if (!resPtr->isNull(indexOfCreated)) {
                proc->setCreated(
                    helpers::parseDateTime(
                        resPtr->curRowColumn(indexOfCreated)
                    )
                );
            }

            if (!resPtr->isNull(indexOfModified)) {
                proc->setUpdated(
                    helpers::parseDateTime(
                        resPtr->curRowColumn(indexOfModified)
                    )
                );
            }

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
        queryResults = _connection->getResults(
            QString("SHOW TRIGGERS FROM ")
            + _connection->quoteIdentifier(dbName));
    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
                << "Failed to fetch triggers: " << ex.message();
        return;
    }

    Query * resPtr = queryResults.get();

    if (resPtr) {

        std::size_t indexOfTrigger = resPtr->indexOfColumn("Trigger");
        std::size_t indexOfCreated = resPtr->indexOfColumn("Created");

        while (resPtr->isEof() == false) {

            QString name = resPtr->curRowColumn(indexOfTrigger);

            TriggerEntity * trigger = new TriggerEntity(name);

            if (!resPtr->isNull(indexOfCreated)) {
                trigger->setCreated(
                    helpers::parseDateTime(
                        resPtr->curRowColumn(indexOfCreated)
                    )
                );
            }

            toList->list()->append(trigger);

            resPtr->seekNext();
        }
    }

}

} // namespace db
} // namespace meow

