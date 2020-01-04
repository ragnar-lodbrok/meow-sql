#include "qtsql_query.h"
#include "db/connection.h"
#include "db/editable_grid_data.h"
#include "db/data_type/connection_data_types.h"

namespace meow {
namespace db {

QtSQLQuery::QtSQLQuery(Connection * connection)
    :Query(connection),
    _columnsParsed(false)
{

}

QtSQLQuery::~QtSQLQuery()
{

}

void QtSQLQuery::execute(bool addResult)
{
    QueryResults results = connection()->query(this->SQL(), true);

    QtSQLQueryResultPtr lastResult = nullptr;
    if (!results.isEmpty()) {
        lastResult
                = std::static_pointer_cast<QtSQLQueryResult>(results.front());
    }

    if (addResult && _resultList.size() == 0) {
        addResult = false;
    }

    if (addResult == false) {
        _resultList.clear();
        _recordCount = 0;
        // H: ...
        if (isEditing()) {
            _editableData->clear();
        }
    }

    if (lastResult) {
        _resultList.push_back(lastResult);
        _recordCount += lastResult->rowsCount();
    }

    if (!addResult) {
        clearColumnData(); // TODO
        if (_resultList.empty() == false) {
            // H: FCurrentResults := LastResult;

            addColumnData(lastResult); // TODO

            if (isEditing()) {
                //prepareResultForEditing(lastResult);
                lastResult.reset(); // we just copied all data
            }

            seekFirst();
        }
    } else {
        if (lastResult && isEditing()) {
            //prepareResultForEditing(lastResult);
            lastResult.reset();
        }
    }
}

bool QtSQLQuery::hasResult()
{
    return _resultList.empty() == false;
}

void QtSQLQuery::seekRecNo(db::ulonglong value)
{
    if (value == _curRecNo) {
        return;
    }

    if (value >= recordCount()) {
        _curRecNo = recordCount();
        _eof = true;
        return;
    }

    if (isEditing() == false) {
        db::ulonglong numRows = 0;
        for (auto result : _resultList) {

            db::ulonglong resultNumRows = result->rowsCount();
            numRows += resultNumRows;

            if (value < numRows) {
                _currentResult = result;
                // TODO: using unsigned with "-" is risky
                _curRecNoLocal = resultNumRows - (numRows - value);

                result->query()->seek(_curRecNoLocal);

                break;
            }
        }
    }

    _curRecNo = value;
    _eof = false;
}

QString QtSQLQuery::curRowColumn(std::size_t index,
                             bool ignoreErrors)
{
    if (index < columnCount()) {

        if (isEditing()) {
            return _editableData->dataAt(_curRecNo, index);
        }

        // TODO: binary

        return _currentResult->query()->value(index).toString();

    } else if (!ignoreErrors) {
        throw db::Exception(QString(
            "Column #%1 not available. Query returned %2 columns and %3 rows.")
            .arg(index).arg(columnCount()).arg(recordCount()
        ));
    }

    return QString();
}

bool QtSQLQuery::isNull(std::size_t index)
{
    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return _currentResult->query()->isNull(index);
}

bool QtSQLQuery::prepareEditing()
{
    return true;
}

void QtSQLQuery::clearColumnData()
{
    _columns.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void QtSQLQuery::addColumnData(QtSQLQueryResultPtr & result)
{
    if (_columnsParsed) return;

    QSqlRecord rec = result->query()->record();

    unsigned int numFields = static_cast<unsigned int>(rec.count());

    _columns.resize(numFields);

    for (unsigned int i=0; i < numFields; ++i) {
        QueryColumn & column = _columns[i];
        QString fieldName = rec.fieldName(static_cast<int>(i));
        column.name = fieldName;  // TODO can we get original table name?
        column.orgName = fieldName;
        _columnIndexes.insert(fieldName, i);
        column.dataType = connection()->dataTypes()->dataTypeFromField(
                rec.field(static_cast<int>(i)));
    }

    _columnsParsed = true;
}

} // namespace db
} // namespace meow
