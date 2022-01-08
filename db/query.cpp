#include "query.h"
#include "exception.h"
#include "editable_grid_data.h"
#include "entity/table_entity.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

Query::Query(Connection * connection)
    :_rowsFound(0),
     _rowsAffected(0),
     _warningsCount(0),
     _execDuration(std::chrono::milliseconds(0)),
     _networkDuration(std::chrono::milliseconds(0)),
     _connection(connection),
     _entity(nullptr)
{

}

Query::~Query()
{

}

void Query::setSQL(const QString & SQL)
{
    _SQL = SQL;
}

void Query::execute(bool appendData)
{
    // TODO appendData for isEditing() is broken

    QueryResults results = connection()->query(this->SQL(), true);

    if (_entity) {
        for (QueryResultPt & result : results.list()) {
            result->setEntity(_entity);
        }
    }

    if (!_currentResult) {
        appendData = false;
    }

    if (appendData) {

        _rowsFound += results.rowsFound();
        _rowsAffected += results.rowsAffected();
        _warningsCount += results.warningsCount();
        _execDuration = +results.execDuration();
        _networkDuration = +results.networkDuration();

        Q_ASSERT(results.list().size() <= 1); // only 1 result can be added

        if (!results.isEmpty()) {
            _currentResult->appendResultData(results.front());
        }

    } else {

        _rowsFound = results.rowsFound();
        _rowsAffected = results.rowsAffected();
        _warningsCount = results.warningsCount();
        _execDuration = results.execDuration();
        _networkDuration = results.networkDuration();

        _resultList = results.list();
        _currentResult = nullptr;
        if (!results.isEmpty()) {
            _currentResult = results.front();
        }
    }
}


} // namespace db
} // namespace meow

