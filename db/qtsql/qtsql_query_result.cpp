#include "qtsql_query_result.h"
#include "db/editable_grid_data.h"
#include "db/connection.h"
#include "db/data_type/connection_data_types.h"

namespace meow {
namespace db {

void QtSQLQueryResult::init(QSqlQuery * query, QSqlDatabase * database)
{
    // Q_ASSERT(query->nextResult() == false);
    // TODO: multiple result sets support pbly at above level

    Q_ASSERT(_query == nullptr);
    Q_ASSERT( query != nullptr);

    _query = query;
    _database = database;

    _recordCount = nativeRowsCount();

    clearColumnData();

    addColumnData(_query);

    if (isEditing()) {
        prepareResultForEditing(this);
    }

    seekFirst();
}

db::ulonglong QtSQLQueryResult::nativeRowsCount() const
{

    if (_database->driver()->hasFeature(QSqlDriver::QuerySize)) { // fast

        return static_cast<db::ulonglong>(_query->size());

    } else { // slow
        if (_rowsCountCache == static_cast<db::ulonglong>(-1)) {
            _query->last();
            int currentRow = _query->at();
            if (currentRow < 0) { // on error this value is -2 in sqlite
                _rowsCountCache = 0;
            } else {
                _rowsCountCache = static_cast<db::ulonglong>(currentRow + 1);
            }

            // TODO: seek first?
            //_query->first();
        }
        return _rowsCountCache;
    }
}

bool QtSQLQueryResult::hasData() const
{
    return _query != nullptr || NativeQueryResult::hasData();
}

void QtSQLQueryResult::seekRecNo(db::ulonglong value)
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
        std::vector<const QtSQLQueryResult *> resultList = this->resultList();
        for (const QtSQLQueryResult * result : resultList) {

            db::ulonglong resultNumRows = result->nativeRowsCount();
            numRows += resultNumRows;

            if (value < numRows) {
                // TODO: using unsigned with "-" is risky
                db::ulonglong curRecNoLocal = resultNumRows - (numRows - value);
                _currentQuery = result->query();
                _currentQuery->seek(curRecNoLocal);

                break;
            }
        }
    }

    _curRecNo = value;
    _eof = false;
}

QString QtSQLQueryResult::curRowColumn(std::size_t index, bool ignoreErrors)
{
    if (index < columnCount()) {

        if (isEditing()) {
            return _editableData->dataAt(_curRecNo, index);
        }

        DataTypeCategoryIndex typeCategory
            = column(index).dataType->categoryIndex;
        if (typeCategory == DataTypeCategoryIndex::Binary) {
            // TODO: more effective way?
            QByteArray bytes
                    = _currentQuery->value(index).toByteArray();

            return QString::fromLatin1(bytes.constData(), bytes.length());
        }

        return _currentQuery->value(index).toString();

    } else if (!ignoreErrors) {
        throw db::Exception(QString(
            "Column #%1 not available. Query returned %2 columns and %3 rows.")
            .arg(index).arg(columnCount()).arg(recordCount()
        ));
    }

    return QString();
}

bool QtSQLQueryResult::isNull(std::size_t index)
{
    throwOnInvalidColumnIndex(index);

    if (isEditing()) {
        return _editableData->dataAt(_curRecNo, index).isNull();
    }

    return _currentQuery->isNull(index);
}

void QtSQLQueryResult::prepareResultForEditing(NativeQueryResult * result)
{
    Q_UNUSED(result);
    // TODO: not implemented
}

void QtSQLQueryResult::clearColumnData()
{
    _columns.clear();
    _columnIndexes.clear();
    _columnsParsed = false;
}

void QtSQLQueryResult::addColumnData(QSqlQuery * query)
{
    if (_columnsParsed) return;

    QSqlRecord rec = query->record();

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

std::vector<const QtSQLQueryResult *> QtSQLQueryResult::resultList() const
{
    std::vector<const QtSQLQueryResult *> resultList;
    resultList.reserve(_appendedResults.size() + 1);
    if (_query) {
        resultList.push_back(static_cast<const QtSQLQueryResult *>(this));
    }

    for (const QueryResultPt & appendedResult : _appendedResults) {
        resultList.push_back(
            static_cast<const QtSQLQueryResult *>(appendedResult.get())
        );
    }
    return resultList;
}

} // namespace db
} // namespace meow
