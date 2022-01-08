#ifndef DB_QUERY_H
#define DB_QUERY_H

#include <vector>
#include <chrono>
#include <QStringList>
#include <QMap>

#include "common.h"
#include "query_column.h"
#include "native_query_result.h"

namespace meow {
namespace db {

class Connection;
class EditableGridData;
class Entity;

// Executes query and stories execution result.
// Some queries can have multiple result states, but be default Query provides
// data for first result.
class Query
{
public:
    Query(Connection * connection = nullptr);
    virtual ~Query();

    void setSQL(const QString & SQL);
    void setConnection(Connection * connection) { _connection = connection; }

    const QString & SQL() const { return _SQL; }
    Connection * connection() const { return _connection; }

    inline db::ulonglong recordCount() const {
        if (!_currentResult) return 0;
        return _currentResult->recordCount();
    }
    inline bool isEof() const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->isEof();
    }

    inline std::size_t columnCount() const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->columnCount();
    }

    inline QString columnName(std::size_t index) const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->columnName(index);
    }

    inline QStringList columnOrgNames() const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->columnOrgNames();
    }

    inline QueryColumn & column(std::size_t index) {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->column(index);
    }

    // H: procedure Execute(AddResult: Boolean=False; UseRawResult: Integer=-1); virtual; abstract;
    void execute(bool appendData = false);

    inline bool hasResult() {
        return _resultList.empty() == false;
    }

    size_t resultCount() const {
        return _resultList.size();
    }

    QueryResultPt resultAt(size_t index) {
        return _resultList.at(index);
    }

    inline void seekRecNo(db::ulonglong value) { // H: SetRecNo
        Q_ASSERT(_currentResult != nullptr);
        _currentResult->seekRecNo(value);
    }

    inline QString curRowColumn(std::size_t index, bool ignoreErrors = false) {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->curRowColumn(index, ignoreErrors);
    }

    inline QString curRowColumn(const QString & colName, bool ignoreErrors = false) {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->curRowColumn(colName, ignoreErrors);
    }
    inline QStringList curRow() {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->curRow();
    }
    inline QMap<QString, QString> curRowAsObject() {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->curRowAsObject();
    }

    inline bool isNull(std::size_t index) {  // TODO: add by name mthd
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->isNull(index);
    }

    // true if was already prepared
    inline bool prepareEditing() {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->prepareEditing();
    }
    inline bool isEditing() const {
        if (!_currentResult) return false;
        return _currentResult->isEditing();
    }
    inline EditableGridData * editableData() const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->editableData();
    }

    inline void seekFirst() {
        Q_ASSERT(_currentResult != nullptr);
        _currentResult->seekFirst();
    }
    inline void seekNext() {
        Q_ASSERT(_currentResult != nullptr);
        _currentResult->seekNext();
    }

    inline std::size_t indexOfColumn(const QString & colName) const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->indexOfColumn(colName);
    }

    inline Entity * entity() const {
        return _entity;
    }
    void setEntity(Entity * entity) {
        _entity = entity;
        for (QueryResultPt & result : _resultList) {
            result->setEntity(_entity);
        }
    }

    inline QStringList keyColumns() const {
        Q_ASSERT(_currentResult != nullptr);
        return _currentResult->keyColumns();
    }

    inline db::ulonglong rowsFound() const {
        return _rowsFound;
    }

    inline db::ulonglong rowsAffected() const {
        return _rowsAffected;
    }

    inline db::ulonglong warningsCount() const {
        return _warningsCount;
    }

    inline std::chrono::milliseconds execDuration() const {
        return _execDuration;
    }

    inline std::chrono::milliseconds networkDuration() const {
        return _networkDuration;
    }

protected:

    db::ulonglong _rowsFound;
    db::ulonglong _rowsAffected;
    db::ulonglong _warningsCount;
    std::chrono::milliseconds _execDuration;
    std::chrono::milliseconds _networkDuration;

private:
    QString _SQL;
    Connection * _connection;
    Entity * _entity;

    std::vector<QueryResultPt> _resultList;
    QueryResultPt _currentResult;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_H
