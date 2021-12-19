#include "pg_query.h"
#include "db/editable_grid_data.h"
#include "db/data_type/pg_connection_data_types.h"

namespace meow {
namespace db {

PGQuery::PGQuery(Connection *connection)
    :Query(connection),
    _columnsParsed(false)
{

}

PGQuery::~PGQuery()
{

}

void PGQuery::execute(bool addResult)
{
    // TODO addResult for isEditing() is broken

    QueryResults results = connection()->query(this->SQL(), true);

    _rowsFound = results.rowsFound();
    _rowsAffected = results.rowsAffected();
    _warningsCount = results.warningsCount();
    _execDuration = results.execDuration();
    _networkDuration = results.networkDuration();

    PGQueryResultPtr lastResult = nullptr;
    if (!results.isEmpty()) {
        lastResult = std::static_pointer_cast<PGQueryResult>(results.front());
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
                prepareResultForEditing(lastResult);
                lastResult.reset(); // we just copied all data
            }

            seekFirst();
        }
    } else {
        if (lastResult && isEditing()) {
            prepareResultForEditing(lastResult);
            lastResult.reset();
        }
    }
}

bool PGQuery::hasResult()
{
    return _resultList.empty() == false;
}

void PGQuery::seekRecNo(db::ulonglong value)
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

            if (numRows > value) {
                _currentResult = result;
                // TODO: using unsigned with "-" is risky
                _curRecNoLocal = resultNumRows - (numRows - value);

                for (size_t i=0; i<_columnLengths.size(); ++i) {
                    _columnLengths[i] = static_cast<unsigned int>(
                        PQgetlength(
                            _currentResult->nativePtr(),
                            static_cast<int>(_curRecNoLocal),
                            static_cast<int>(i)
                        )
                    );
                }

                break;
            }
        }
    }

    _curRecNo = value;
    _eof = false;
}

QString PGQuery::curRowColumn(std::size_t index,
                             bool ignoreErrors)
{
    // TODO: hide this check for release builds?
    if (index < columnCount()) {

        if (isEditing()) {
            return _editableData->dataAt(_curRecNo, index);
        }


        return rowDataToString(_currentResult->nativePtr(),
                               static_cast<int>(_curRecNoLocal),
                               static_cast<int>(index),
                               static_cast<int>(_columnLengths[index]));

    } else if (!ignoreErrors) {
        throw db::Exception(QString(
            "Column #%1 not available. Query returned %2 columns and %3 rows.")
            .arg(index).arg(columnCount()).arg(recordCount()
        ));
    }

    return QString();
}

QString PGQuery::rowDataToString(PGresult * result,
                        int row,
                        int col,
                        int dataLen)
{

    // PQgetvalue() returns "" for NULL values, so we need this:
    if (PQgetisnull(result, row, col)) {
        return QString(); // Null str
    }

    QString str;

    auto typeCategory = column(col).dataType->categoryIndex;
    if (typeCategory == DataTypeCategoryIndex::Binary
        || typeCategory == DataTypeCategoryIndex::Spatial) {
        str = QString::fromLatin1(
                    PQgetvalue(result, row, col),
                    dataLen);
    // } else if (bool) { // TODO

    } else {
        // TODO: non-unicode support?
        str = QString::fromUtf8(
                    PQgetvalue(result, row, col),
                    dataLen);
    }

    return str;
}


bool PGQuery::isNull(std::size_t index)
{
    //throwOnInvalidColumnIndex(index); // TODO

    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return PQgetisnull(
        _currentResult->nativePtr(),
        static_cast<int>(_curRecNoLocal),
        static_cast<int>(index)
    ) == 1;
}

bool PGQuery::prepareEditing()
{
    if (Query::prepareEditing()) {
        return true;
    }

    for (auto result : _resultList) {
        addColumnData(result);
        prepareResultForEditing(result);
        result.reset();
    }

    return false;
}

void PGQuery::clearColumnData()
{
    _columns.clear();
    _columnLengths.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void PGQuery::addColumnData(PGQueryResultPtr & result)
{
    if (_columnsParsed) return;

    auto types = static_cast<PGConnectionDataTypes *>(
                connection()->dataTypes());

    PGresult * nativeRes = result->nativePtr();

    unsigned int numFields = static_cast<unsigned int>(PQnfields(nativeRes));

    _columnLengths.resize(numFields);
    // TODO: skip columns parsing when we don't need them (e.g. sample queries)

    _columns.resize(numFields);


    for (unsigned int i=0; i < numFields; ++i) {
        QueryColumn & column = _columns[i];
        QString fieldName = QString(PQfname(nativeRes, i));
        column.name = fieldName;  // TODO can we get original table name?
        column.orgName = fieldName;
        _columnIndexes.insert(fieldName, i);
        // TODO PQftable ?
        // TODO PQftablecol
        Oid type = PQftype(nativeRes, i);
        column.dataType = types->dataTypeFromNative(type);

    }

    _columnsParsed = true;
}

void PGQuery::prepareResultForEditing(PGQueryResultPtr & result)
{
    PGresult * nativeRes = result->nativePtr();

    int numRows = PQntuples(nativeRes);
    int numCols = PQnfields(nativeRes);

    _editableData->reserveForAppend(numRows);

    for (int row = 0; row < numRows; ++row) {
        GridDataRow rowData;
        rowData.reserve(numCols);

        for (int col = 0; col < numCols; ++col) {

            int cellLength = PQgetlength(nativeRes, row, col);
            rowData.append(
                rowDataToString(nativeRes, row, col, cellLength)
            );
        }

        _editableData->appendRow(rowData);
    }
}

} // namespace db
} // namespace meow
