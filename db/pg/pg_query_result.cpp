#include "pg_query_result.h"
#include "db/editable_grid_data.h"
#include "db/data_type/pg_connection_data_types.h"
#include "db/pg/pg_connection.h"

namespace meow {
namespace db {

void PGQueryResult::init(PGresult * result, PGconn * connectionHandle)
{
    Q_ASSERT(_res == nullptr);

    _res = result;
    _connectionHandle = connectionHandle;

    _recordCount = nativeRowsCount();

    clearColumnData();

    addColumnData(_res);

    if (isEditing()) {
        prepareResultForEditing(this);
    }

    seekFirst();
}

bool PGQueryResult::hasData() const
{
    return _res != nullptr || NativeQueryResult::hasData();
}

void PGQueryResult::seekRecNo(db::ulonglong value)
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

        std::vector<PGresult *> resultList = this->resultList();

        db::ulonglong numRows = 0;
        for (PGresult * result : resultList) {

            db::ulonglong resultNumRows = PQntuples(result);
            numRows += resultNumRows;

            if (numRows > value) {
                _currentResult = result;
                // TODO: using unsigned with "-" is risky
                _curRecNoLocal = resultNumRows - (numRows - value);

                for (size_t i=0; i<_columnLengths.size(); ++i) {
                    _columnLengths[i] = static_cast<unsigned int>(
                        PQgetlength(
                            _currentResult,
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

QString PGQueryResult::curRowColumn(std::size_t index,
                                    bool ignoreErrors)
{
    // TODO: hide this check for release builds?
    if (index < columnCount()) {

        if (isEditing()) {
            return _editableData->dataAt(_curRecNo, index);
        }


        return rowDataToString(_currentResult,
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

bool PGQueryResult::isNull(std::size_t index)
{
    throwOnInvalidColumnIndex(index);

    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return PQgetisnull(
        _currentResult,
        static_cast<int>(_curRecNoLocal),
        static_cast<int>(index)
    ) == 1;
}

void PGQueryResult::prepareResultForEditing(NativeQueryResult * result)
{
    PGresult * nativeRes = static_cast<PGQueryResult *>(result)->nativePtr();

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

void PGQueryResult::clearColumnData()
{
    _columns.clear();
    _columnLengths.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void PGQueryResult::addColumnData(PGresult * result)
{
    if (_columnsParsed) return;

    auto types = static_cast<PGConnectionDataTypes *>(
                connection()->dataTypes());

    unsigned int numFields = static_cast<unsigned int>(PQnfields(result));

    _columnLengths.resize(numFields);
    // TODO: skip columns parsing when we don't need them (e.g. sample queries)

    _columns.resize(numFields);


    for (unsigned int i=0; i < numFields; ++i) {
        QueryColumn & column = _columns[i];
        QString fieldName = QString(PQfname(result, i));
        column.name = fieldName;  // TODO can we get original table name?
        column.orgName = fieldName;
        _columnIndexes.insert(fieldName, i);
        // TODO PQftable ?
        // TODO PQftablecol
        Oid type = PQftype(result, i);
        column.dataType = types->dataTypeFromNative(type);

    }

    _columnsParsed = true;
}

std::vector<PGresult *> PGQueryResult::resultList() const
{
    // TODO: optimize/cache resultList?
    std::vector<PGresult *> resultList;
    resultList.reserve(_appendedResults.size() + 1);
    if (_res) {
        resultList.push_back(_res);
    }

    for (const QueryResultPt & appendedResult : _appendedResults) {
        resultList.push_back(
            static_cast<PGQueryResult *>(appendedResult.get())
                    ->nativePtr()
        );
    }
    return resultList;
}

void PGQueryResult::clearAll() {
    while (_res != nullptr) {
        PQclear(_res);
        _res = PQgetResult(_connectionHandle);
    }
}

QString PGQueryResult::rowDataToString(PGresult * result,
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

} // namespace db
} // namespace meow
