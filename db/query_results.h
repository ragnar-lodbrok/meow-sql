#ifndef DB_QUERY_RESULTS_H
#define DB_QUERY_RESULTS_H

#include <chrono>
#include <vector>
#include "native_query_result.h"

namespace meow {
namespace db {

using QueryResultList = std::vector<QueryResultPt>;

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
        return _list.empty();
    }

    inline void clear() {
        _list.clear();
        _rowsFound = 0;
        _rowsAffected = 0;
        _warningsCount = 0;
        _execDuration = std::chrono::milliseconds(0);
        _networkDuration = std::chrono::milliseconds(0);
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

    inline std::chrono::milliseconds execDuration() const {
        return _execDuration;
    }

    inline void setExecDuration(std::chrono::milliseconds duration) {
        _execDuration = duration;
    }

    inline void incExecDuration(std::chrono::milliseconds duration) {
        _execDuration += duration;
    }

    inline std::chrono::milliseconds networkDuration() const {
        return _networkDuration;
    }

    inline void setNetworkDuration(std::chrono::milliseconds duration) {
        _networkDuration = duration;
    }

    inline void incNetworkDuration(std::chrono::milliseconds duration) {
        _networkDuration += duration;
    }

    inline QueryResults & operator<< (const QueryResultPt &result) {
        _list.push_back(result);
        return *this;
    }

    inline const QueryResultPt & operator[](int i) const {
        return _list[i];
    }

    inline const QueryResultPt & at(int i) const {
        return _list.at(i);
    }

    inline QueryResultPt& front() {
        return _list.front();
    }

    inline const QueryResultPt& front() const {
        return _list.front();
    }

private:
    QueryResultList _list;
    db::ulonglong _rowsFound = 0;
    db::ulonglong _rowsAffected = 0;
    db::ulonglong _warningsCount = 0;
    std::chrono::milliseconds _execDuration = std::chrono::milliseconds(0);
    std::chrono::milliseconds _networkDuration = std::chrono::milliseconds(0);
};


} // namespace db
} // namespace meow

#endif // DB_QUERY_RESULTS_H
