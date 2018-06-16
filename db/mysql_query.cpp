#include <QDebug>
#include "mysql_query.h"
#include "data_type/mysql_data_type.h"

namespace meow {
namespace db {

MySQLQuery::MySQLQuery(Connection *connection)
    :Query(connection),
     _curRow(nullptr)
{

}

MySQLQuery::~MySQLQuery()
{

}

void MySQLQuery::execute(bool addResult /*= false*/) // override
{
    qDebug() << "[MySQLQuery] " << "Executing: " << SQL();

    QueryResults results = connection()->query(this->SQL(), true);

    MySQLQueryResultPt lastResult = nullptr;
    if (!results.isEmpty()) {
        lastResult = std::static_pointer_cast<MySQLQueryResult>(results.front());
    }

    if (addResult && _resultList.size() == 0) {
        addResult = false;
    }

    if (addResult == false) {
        _resultList.clear();
        _recordCount = 0;
        // H: ...
    }

    if (lastResult) {
        _resultList.push_back(lastResult);
        _recordCount += lastResult->nativePtr()->row_count;
    }

    if (!addResult) {
        _columns.clear();
        _columnLengths.clear();
        _columnIndexes.clear();
        if (_resultList.empty() == false) {
            // FCurrentResults is normally done in SetRecNo, but never if result has no rows
            // H: FCurrentResults := LastResult;

            unsigned int numFields = mysql_num_fields(lastResult->nativePtr());

            _columnLengths.resize(numFields);
            // TODO: skip columns parsing when we don't need them (e.g. sample queries)

            _columns.resize(numFields);

            for (unsigned int i=0; i < numFields; ++i) {
                MYSQL_FIELD * field = mysql_fetch_field_direct(lastResult->nativePtr(), i);
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
                column.dataTypeIndex = dataTypeOfField(field);
                column.dataTypeCategoryIndex
                    = meow::db::categoryOfDataType(column.dataTypeIndex);
            }

            seekFirst();
        }
    }
}

DataTypeIndex MySQLQuery::dataTypeOfField(MYSQL_FIELD * field)
{
    if (field->flags & ENUM_FLAG) {
        return DataTypeIndex::Enum;
    } else if (field->flags & SET_FLAG) {
        return DataTypeIndex::Set;
    }

    // TODO: detect binary

    return dataTypeFromMySQLDataType(field->type);
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
    bool editingPrepared = false; // TODO
    if (!editingPrepared && (value >= _recordCount) ) {
        _curRecNo = _recordCount;
        _eof = true;
        return;
    }
    bool rowFound = false;
    if (editingPrepared) {
        // TODO
    }

    if (!rowFound) {
        db::ulonglong numRows = 0;
        for (auto result : _resultList) {
            numRows += result->nativePtr()->row_count;
            if (numRows > value) {
                _currentResult = result; // TODO: why ?
                MYSQL_RES * curResPtr = _currentResult->nativePtr();
                // TODO: using unsigned with "-" is risky
                db::ulonglong wantedLocalRecNo = curResPtr->row_count - (numRows - value);
                // H: Do not seek if FCurrentRow points to the previous row of the wanted row
                if (wantedLocalRecNo == 0 || (_curRecNo+1 != value) || _curRow == nullptr) {
                    // TODO: it does not seems we need 3rd condition?
                    mysql_data_seek(curResPtr, wantedLocalRecNo);
                }

                _curRow = mysql_fetch_row(curResPtr);

                // H: FCurrentUpdateRow := nil;

                // H: Remember length of column contents. Important for Col() so contents
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
        // H: if FEditingPrepared // TODO

        QString result;

        DataTypeCategoryIndex typeCategory = column(index).dataTypeCategoryIndex;
        if (typeCategory == DataTypeCategoryIndex::Binary
            || typeCategory == DataTypeCategoryIndex::Spatial) {
            result = QString::fromLatin1(_curRow[index], _columnLengths[index]);
        } else {
            // TODO: non-unicode support?
            result = QString::fromUtf8(_curRow[index], _columnLengths[index]);
        }

        // H: if Datatype(Column).Index = dtBit // TODO

        return result;

    } else if (!ignoreErrors) {
        throw db::Exception(
            QString("Column #%1 not available. Query returned %2 columns and %3 rows.")
                    .arg(index).arg(columnCount()).arg(recordCount())
        );
    }

    return QString();
}

bool MySQLQuery::isNull(std::size_t index)
{
    throwOnInvalidColumnIndex(index);

    return _curRow[index] == nullptr;
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

} // namespace db
} // namespace meow

