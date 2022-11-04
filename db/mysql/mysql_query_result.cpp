#include "mysql_query_result.h"
#include "db/editable_grid_data.h"
#include "db/connection.h"
#include "db/data_type/mysql_data_type.h"
#include "db/data_type/mysql_connection_data_types.h"

namespace meow {
namespace db {

MySQLQueryResult::MySQLQueryResult(Connection * connection)
    : NativeQueryResult(connection)
    , _res(nullptr)
    , _curRow(nullptr)
    , _columnsParsed(false)
{

}

void MySQLQueryResult::init(MYSQL_RES * res)
{
    Q_ASSERT( res != nullptr);
    Q_ASSERT(_res == nullptr);

    _res = res;

    _recordCount = nativeRowsCount();

    clearColumnData();

    addColumnData(_res);

    if (isEditing()) {
        prepareResultForEditing(this);
    }

    seekFirst();
}

void MySQLQueryResult::clearColumnData()
{
    _columns.clear();
    _columnLengths.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void MySQLQueryResult::addColumnData(MYSQL_RES * result)
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

bool MySQLQueryResult::hasData() const
{
    return _res != nullptr || NativeQueryResult::hasData();
}

void MySQLQueryResult::seekRecNo(db::ulonglong value)
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

        std::vector<MYSQL_RES *> resultList = this->resultList();

        db::ulonglong numRows = 0;
        for (auto curResPtr : resultList) {
            numRows += curResPtr->row_count;
            if (value < numRows) {
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

QString MySQLQueryResult::curRowColumn(std::size_t index, bool ignoreErrors)
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

bool MySQLQueryResult::isNull(std::size_t index)
{
    throwOnInvalidColumnIndex(index);

    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return _curRow[index] == nullptr;
}

bool MySQLQueryResult::columnIsPrimaryKeyPart(std::size_t index) const
{
    // faster than in base class and works for any query
    return (column(index).flags & PRI_KEY_FLAG) > 0;
}

bool MySQLQueryResult::columnIsUniqueKeyPart(std::size_t index) const
{
    // Listening: Fn .380 Acp #19074 - 1914
    return (column(index).flags & UNIQUE_KEY_FLAG) > 0;
    // TODO: sometimes the flag is not set, probably bug in mysqlclient?
}

bool MySQLQueryResult::columnIsIndexKeyPart(std::size_t index) const
{
    return (column(index).flags & MULTIPLE_KEY_FLAG) > 0;
}

bool MySQLQueryResult::columnIsAutoIncrement(std::size_t index) const
{
    return (column(index).flags & AUTO_INCREMENT_FLAG) > 0;
}

std::vector<MYSQL_RES *> MySQLQueryResult::resultList() const
{
    // TODO: optimize/cache resultList?
    std::vector<MYSQL_RES *> resultList;
    resultList.reserve(_appendedResults.size() + 1);
    if (_res) {
        resultList.push_back(_res);
    }

    for (const QueryResultPt & appendedResult : _appendedResults) {
        resultList.push_back(
            static_cast<MySQLQueryResult *>(appendedResult.get())
                    ->nativePtr()
        );
    }
    return resultList;
}

QString MySQLQueryResult::rowDataToString(MYSQL_ROW row,
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

void MySQLQueryResult::prepareResultForEditing(MYSQL_RES * result)
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


void MySQLQueryResult::prepareResultForEditing(NativeQueryResult * result)
{
    MySQLQueryResult * mysqlResult = static_cast<MySQLQueryResult *>(result);
    if (mysqlResult->nativePtr()) {
        prepareResultForEditing(mysqlResult->nativePtr());
        mysqlResult->freeNative(); // we just copied all data
    }
}

} // namespace db
} // namespace meow

