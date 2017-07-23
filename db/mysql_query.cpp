#include <QDebug>
#include "mysql_query.h"

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

void MySQLQuery::execute(bool addResult /*= false*/, std::size_t useRawResult /*= -1*/) // override
{
    qDebug() << "[MySQLQuery] " << "Executing: " << SQL();

    // Execute a query, or just take over one of the last result pointers
    if (useRawResult == (std::size_t)-1) {
        connection()->query(this->SQL(), true); //H: FStoreResult seems always true?
        useRawResult = 0;
    }

    MySQLResult lastResult = nullptr;
    if (connection()->lastResultsCount() > useRawResult) {
        lastResult = static_cast<MySQLConnection *>(connection())->lastRawResultAt(useRawResult);
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
        _recordCount += lastResult.get()->row_count;
    }

    qDebug() << "record count" << _recordCount;

    if (!addResult) {
        if (_resultList.empty() == false) {
            // FCurrentResults is normally done in SetRecNo, but never if result has no rows
            // H: FCurrentResults := LastResult;

            unsigned int numFields = mysql_num_fields(lastResult.get());

            //H: SetLength(FColumnTypes, NumFields);
            //H: SetLength(FColumnFlags, NumFields);

            _columnLengths.resize(numFields);

            _columnNames.clear(); // TODO: try reserve() ?
            _columnOrgNames.clear();
            _columnIndexes.clear();
            for (unsigned int i=0; i < numFields; ++i) {
                MYSQL_FIELD * field = mysql_fetch_field_direct(lastResult.get(), i);
                QString fieldName = QString(field->name);
                _columnNames.append(fieldName);
                _columnIndexes.insert(fieldName, i);
                if (connection()->serverVersionInt() >= 40100) {
                    _columnOrgNames.append(QString(field->org_name));
                } else {
                    _columnOrgNames.append(fieldName);
                }
            }

            seekFirst();
        } else {
            _columnLengths.clear();
        }
    }
}

bool MySQLQuery::hasResult() // override
{
    return _resultList.empty() == false;
}

void MySQLQuery::seekRecNo(db::ulonglong value) // override
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
            numRows += result.get()->row_count;
            if (numRows > value) {
                _currentResult = result; // TODO: why ?
                MYSQL_RES * curResPtr = _currentResult.get();
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

QString MySQLQuery::curRowColumn(std::size_t index, bool ignoreErrors /*= false*/) // override
{
    if (index < columnCount()) {
        // H: if FEditingPrepared
        // H: if Datatype(Column).Category in [dtcBinary, dtcSpatial] then
        QString result = QString::fromUtf8(_curRow[index], _columnLengths[index]);

        // H: if Datatype(Column).Index = dtBit

        return result;

    } else if (!ignoreErrors) {
        throw db::Exception(
            QString("Column #%1 not available. Query returned %2 columns and %3 rows.")
                    .arg(index).arg(columnCount()).arg(recordCount())
        );
    }

    return QString();
}

bool MySQLQuery::isNull(std::size_t index) // override
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

