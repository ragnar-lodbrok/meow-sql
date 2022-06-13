#ifndef DB_QUERY_CRITERIA_H
#define DB_QUERY_CRITERIA_H

#include "common.h"
#include <QStringList>
#include <QVector>

namespace meow {
namespace db {

class QueryCriteria
{
public:
    QueryCriteria();

    struct SortColumn
    {
        QString columnName;
        bool isAsc = true;
    };

    QStringList select;
    QString quotedDbAndTableName;
    QString where;
    db::ulonglong limit;
    db::ulonglong offset;
    QVector<SortColumn> sortColumns;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_CRITERIA_H
