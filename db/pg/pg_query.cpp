#include "pg_query.h"
#include "db/editable_grid_data.h"

namespace meow {
namespace db {

PGQuery::PGQuery(Connection *connection)
    :Query(connection)
{

}

PGQuery::~PGQuery()
{

}

void PGQuery::execute(bool addResult)
{
    // TODO addResult for isEditing() is broken

    QueryResults results = connection()->query(this->SQL(), true);

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
            Q_ASSERT(false);
            _editableData->clear();
        }
    }

    if (lastResult) {
        _resultList.push_back(lastResult);
        _recordCount += lastResult->rowsCount();
    }

    if (!addResult) {
       // clearColumnData(); // TODO
        if (_resultList.empty() == false) {
            // H: FCurrentResults := LastResult;

            //addColumnData(lastResult->nativePtr()); // TODO

            if (isEditing()) {
                //prepareResultForEditing(lastResult->nativePtr());
                lastResult.reset(); // we just copied all data
            }

            seekFirst();
        }
    } else {
        if (lastResult && isEditing()) {
            //prepareResultForEditing(lastResult->nativePtr());
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
    Q_UNUSED(index);
    Q_UNUSED(ignoreErrors);
    Q_ASSERT(false);
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
    Q_ASSERT(false);
    return false;
}

} // namespace db
} // namespace meow
