#include "sqlite_entities_fetcher.h"
#include "db/sqlite/sqlite_connection.h"
#include "db/query.h"
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"
#include "db/entity/entity_factory.h"

// https://www.sqlite.org/cli.html

namespace meow {
namespace db {

SQLiteEntitiesFetcher::SQLiteEntitiesFetcher(SQLiteConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

QList<EntityPtr> SQLiteEntitiesFetcher::run(const QString & dbName)
{

    // TODO: manually?
    //SELECT name FROM sqlite_master
    //WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%'
    //ORDER BY 1

    Q_UNUSED(dbName);

    QList<EntityPtr> list;

    const SQLiteConnection * conn = static_cast<SQLiteConnection *>(_connection);

    QStringList tables = conn->handle()->tables(QSql::Tables);

    for (const QString & tableName : tables) {
        TableEntityPtr table = EntityFactory::createTable(tableName);

        // TODO: table->setRowsCount
        // TODO: table->setDataSize

        list.append(table);
    }

    QStringList views = conn->handle()->tables(QSql::Views);

    for (const QString & viewName : views) {
        ViewEntityPtr view = EntityFactory::createView(viewName);
        list.append(view);
    }

    // TODO: triggers

    return list;

}

} // namespace db
} // namespace meow
