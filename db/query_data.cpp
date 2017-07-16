#include "query_data.h"
#include "query.h"

namespace meow {
namespace db {

QueryData::QueryData()
{

}

int QueryData::rowCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->recordCount();
    }
    return 0;
}

int QueryData::columnCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->columnCount();
    }
    return 0;
}

QString QueryData::columnName(int index) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->columnName(index);
    }
    return QString();
}

QString QueryData::rawDataAt(int row, int column) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        query->seekRecNo(row);
        if (query->isNull(column)) {
            return "(NULL)"; // TODO: const
        } else {
            // TODO: formatting, see AnyGridGetText
            return query->curRowColumn(column, true);
        }
    }
    return QString();
}

} // namespace db
} // namespace meow
