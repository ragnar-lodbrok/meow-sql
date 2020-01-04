#include "sqlite_entities_fetcher.h"
#include "db/sqlite/sqlite_connection.h"
#include "db/query.h"
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"

// https://www.sqlite.org/cli.html

namespace meow {
namespace db {

SQLiteEntitiesFetcher::SQLiteEntitiesFetcher(SQLiteConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void SQLiteEntitiesFetcher::run(const QString & dbName,
                            EntityListForDataBase * toList)
{

    // TODO: manually?
    //SELECT name FROM sqlite_master
    //WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%'
    //ORDER BY 1

    Q_UNUSED(dbName);

    const SQLiteConnection * conn = static_cast<SQLiteConnection *>(_connection);

    QStringList tables = conn->handle()->tables(QSql::Tables);

    for (const QString & tableName : tables) {
        TableEntity * table = new TableEntity(tableName);

        // TODO: table->setRowsCount
        // TODO: table->setDataSize

        toList->list()->append(table);
    }

    QStringList views = conn->handle()->tables(QSql::Views);

    for (const QString & viewName : views) {
        ViewEntity * view = new ViewEntity(viewName);
        toList->list()->append(view);
    }

    // TODO: triggers

}

} // namespace db
} // namespace meow
