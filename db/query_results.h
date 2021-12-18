#ifndef DB_QUERY_RESULTS_H
#define DB_QUERY_RESULTS_H

#include <QList>
#include "native_query_result_interface.h"

namespace meow {
namespace db {

using QueryResultList = QList<QueryResultPt>;

// List of query execution results with additional data
class QueryResults
{
public:

    inline const QueryResultList & list() const {
        return _list;
    }

    inline QueryResultList & list() {
        return _list;
    }

    inline bool isEmpty() const {
        return _list.isEmpty();
    }

    inline void clear() {
        _list.clear();
        _rowsFound = 0;
        _rowsAffected = 0;
        _warningsCount = 0;
    }

    inline db::ulonglong rowsFound() const {
        return _rowsFound;
    }

    void setRowsFound(db::ulonglong rowsFound) {
        _rowsFound = rowsFound;
    }

    inline void incRowsFound(db::ulonglong rowsFound) {
        _rowsFound += rowsFound;
    }

    inline db::ulonglong rowsAffected() {
        return _rowsAffected;
    }

    inline void setRowsffected(db::ulonglong rowsAffected) {
        _rowsAffected = rowsAffected;
    }

    inline void incRowsAffected(db::ulonglong rowsAffected) {
        _rowsAffected += rowsAffected;
    }

    inline db::ulonglong warningsCount() {
        return _warningsCount;
    }

    inline void setWarningsCount(db::ulonglong warningsCount) {
        _warningsCount = warningsCount;
    }

    inline QueryResults & operator<< (const QueryResultPt &result) {
        _list.append(result);
        return *this;
    }

    inline const QueryResultPt & operator[](int i) const {
        return _list[i];
    }

    inline const QueryResultPt & at(int i) const {
        return _list.at(i);
    }

    inline QueryResultPt& front() {
        return _list.first();
    }

    inline const QueryResultPt& front() const {
        return _list.first();
    }

private:
    QueryResultList _list;
    db::ulonglong _rowsFound = 0;
    db::ulonglong _rowsAffected = 0;
    db::ulonglong _warningsCount = 0;
};


} // namespace db
} // namespace meow

#endif // DB_QUERY_RESULTS_H
