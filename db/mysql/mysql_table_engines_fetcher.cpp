#include "mysql_table_engines_fetcher.h"
#include "db/connection.h"
#include "db/query.h"
#include "helpers/logger.h"

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

            bool isDefault = false;
            if (support == "yes" || (isDefault = (support == "default"))) {
                QString name = resPtr->curRowColumn(indexOfEngine);
                if (isDefault) {
                    engines.push_front(name);
                } else {
                    engines << name;
                }
            }

            resPtr->seekNext();
        }

    } catch(meow::db::Exception & ex) {
        meowLogCC(Log::Category::Error, _connection)
            << "[MySQLTableEnginesFetcher] "
            << "Failed to SHOW ENGINES: " << ex.message();
        // TODO: normal fallback

        engines << "InnoDB"
                << "MyISAM"
                << "CSV"
                << "BLACKHOLE"
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
