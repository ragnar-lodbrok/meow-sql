#include "mysql_query.h"
#include "db/editable_grid_data.h"
#include "db/data_type/mysql_data_type.h"
#include "db/data_type/mysql_connection_data_types.h"

namespace meow {
namespace db {

MySQLQuery::MySQLQuery(Connection *connection)
    :Query(connection),
     _curRow(nullptr),
     _columnsParsed(false)
{

}

MySQLQuery::~MySQLQuery()
{

}

void MySQLQuery::execute(bool addResult /*= false*/) // override
{
    // TODO addResult for isEditing() is broken

    QueryResults results = connection()->query(this->SQL(), true);

    _rowsFound = results.rowsFound();
    _rowsAffected = results.rowsAffected();
    _warningsCount = results.warningsCount();

    MySQLQueryResultPtr lastResult = nullptr;
    if (!results.isEmpty()) {
        lastResult = std::static_pointer_cast<MySQLQueryResult>(
                        results.front());
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
        clearColumnData();
        if (_resultList.empty() == false) {
            // FCurrentResults is normally done in SetRecNo,
            // but never if result has no rows
            // H: FCurrentResults := LastResult;

            addColumnData(lastResult->nativePtr());

            if (isEditing()) {
                prepareResultForEditing(lastResult->nativePtr());
                lastResult.reset(); // we just copied all data
            }

            seekFirst();
        }
    } else {
        if (lastResult && isEditing()) {
            prepareResultForEditing(lastResult->nativePtr());
            lastResult.reset();
        }
    }
}

void MySQLQuery::clearColumnData()
{
    _columns.clear();
    _columnLengths.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void MySQLQuery::addColumnData(MYSQL_RES * result)
{
    if (_columnsParsed) return;

    auto types = static_cast<MySQLConnectionDataTypes *>(
                connection()->dataTypes());

    unsigned int numFields = mysql_num_fields(result);

    _columnLengths.resize(numFields);
    // TODO: skip columns parsing when we don't need them (e.g. sample queries)

    _columns.resize(numFields);

    for (unsigned int i=0; i < numFields; ++i) {
        MYSQL_FIELD * field = mysql_fetch_field_direct(result, i);
        QueryColumn & column = _columns[i];
        QString fieldName = QString(field->name);
        column.name = fieldName;
        _columnIndexes.insert(fieldName, i);
        if (connection()->serverVersionInt() >= 40100) {
            column.orgName = QString(field->org_name);
        } else {
            column.orgName = fieldName;
        }
        column.flags = field->flags;
        column.dataType = types->dataTypeOfField(field);
    }

    _columnsParsed = true;
}


bool MySQLQuery::hasResult()
{
    return _resultList.empty() == false;
}

void MySQLQuery::seekRecNo(db::ulonglong value)
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
            numRows += result->nativePtr()->row_count;
            if (value < numRows) {
                _currentResult = result; // TODO: why ?
                MYSQL_RES * curResPtr = _currentResult->nativePtr();
                // TODO: using unsigned with "-" is risky
                db::ulonglong wantedLocalRecNo
                        = curResPtr->row_count - (numRows - value);
                // H: Do not seek if FCurrentRow points to the previous row
                // of the wanted row
                if (wantedLocalRecNo == 0
                        || (_curRecNo+1 != value)
                        || _curRow == nullptr) {
                    // TODO: it does not seems we need 3rd condition?
                    mysql_data_seek(curResPtr, wantedLocalRecNo);
                }

                _curRow = mysql_fetch_row(curResPtr);

                // H: Remember length of column contents.
                // Important for Col() so contents
                // of cells with #0 chars are not cut off
                unsigned long * lengths = mysql_fetch_lengths(curResPtr);

                for (size_t i=0; i<_columnLengths.size(); ++i) {
                    _columnLengths[i] = lengths[i];
                }

                break;
            }
        }
    }

    _curRecNo = value;
    _eof = false;
}

QString MySQLQuery::curRowColumn(std::size_t index, bool ignoreErrors)
{
    if (index < columnCount()) {

        if (isEditing()) {
            return _editableData->dataAt(_curRecNo, index);
        }

        return rowDataToString(_curRow, index, _columnLengths[index]);

    } else if (!ignoreErrors) {
        throw db::Exception(QString(
            "Column #%1 not available. Query returned %2 columns and %3 rows.")
            .arg(index).arg(columnCount()).arg(recordCount()
        ));
    }

    return QString();
}

bool MySQLQuery::isNull(std::size_t index)
{
    throwOnInvalidColumnIndex(index);

    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return _curRow[index] == nullptr;
}

bool MySQLQuery::prepareEditing()
{
    if (Query::prepareEditing()) {
        return true;
    }

    for (auto result : _resultList) {
        addColumnData(result->nativePtr());
        prepareResultForEditing(result->nativePtr());
        result.reset();
    }

    return false;
}

void MySQLQuery::throwOnInvalidColumnIndex(std::size_t index)
{
    if (index >= columnCount()) {
        throw db::Exception(
            QString("Column #%1 not available. Query returned %2 columns.")
                    .arg(index).arg(columnCount())
        );
    }
}

QString MySQLQuery::rowDataToString(MYSQL_ROW row,
                        std::size_t col,
                        unsigned long dataLen)
{
    QString result;

    DataTypeCategoryIndex typeCategory = column(col).dataType->categoryIndex;
    if (typeCategory == DataTypeCategoryIndex::Binary
        || typeCategory == DataTypeCategoryIndex::Spatial) {
        result = QString::fromLatin1(row[col], dataLen);
    } else {
        // TODO: non-unicode support?
        result = QString::fromUtf8(row[col], dataLen);
    }

    return result;
}

void MySQLQuery::prepareResultForEditing(MYSQL_RES * result)
{
    // it seems that copying all data is simplest way as we need to
    // insert/delete rows at top/in the middle of data as well
    // TODO: heidi works other way, maybe it's faster and/or takes less memory

    db::ulonglong numRows = result->row_count;
    unsigned int numCols = mysql_num_fields(result);

    _editableData->reserveForAppend(numRows);

    mysql_data_seek(result, 0);

    MYSQL_ROW rowDataRaw;
    while ((rowDataRaw = mysql_fetch_row(result))) {
        GridDataRow rowData;
        rowData.reserve(numCols);

        unsigned long * lengths = mysql_fetch_lengths(result);

        for (unsigned col = 0; col < numCols; ++col) {
            rowData.append(
                rowDataToString(rowDataRaw, col, lengths[col])
            );
        }

        _editableData->appendRow(rowData);
    }
}

} // namespace db
} // namespace meow

