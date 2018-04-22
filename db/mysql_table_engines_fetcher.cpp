#include "mysql_table_engines_fetcher.h"
#include "connection.h"
#include "query.h"
#include <QDebug>

namespace meow {
namespace db {

MySQLTableEnginesFetcher::MySQLTableEnginesFetcher(Connection * connection)
    : TableEnginesFetcher(connection)
{

}

QStringList MySQLTableEnginesFetcher::run()
{

    QStringList engines;

    _connection->ping(true);
    try {

        QueryPtr enginesResults = _connection->getResults("SHOW ENGINES");
        Query * resPtr = enginesResults.get();
        if (!resPtr) {
            return engines;
        }

        std::size_t indexOfEngine = resPtr->indexOfColumn("Engine");
        std::size_t indexOfSupport = resPtr->indexOfColumn("Support");

        while (resPtr->isEof() == false) {
            QString support = resPtr->curRowColumn(indexOfSupport).toLower();

            if (support == "yes" || support == "default") {
                QString name = resPtr->curRowColumn(indexOfEngine);
                engines << name;
            }

            resPtr->seekNext();
        }

    } catch(meow::db::Exception & ex) {
        qDebug() << "[MySQLTableEnginesFetcher] "
                 << "Failed to SHOW ENGINES: " << ex.message();
        // TODO: normal fallback

        engines << "MyISAM"
                << "CSV"
                << "BLACKHOLE"
                << "InnoDB"
                << "MRG_MYISAM"
                << "PERFORMANCE_SCHEMA"
                << "ARCHIVE"
                << "MEMORY"
                << "FEDERATED"
                << "HEAP"
                << "BDB"
                << "EXAMPLE";
    }
    return engines;
}

} // namespace db
} // namespace meow
