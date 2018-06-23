#ifndef DB_QUERY_CRITERIA_H
#define DB_QUERY_CRITERIA_H

#include "common.h"
#include <QStringList>

namespace meow {
namespace db {

class QueryCriteria
{
public:
    QueryCriteria();

    QStringList select;
    QString quotedDbAndTableName;
    db::ulonglong limit;
    db::ulonglong offset;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_CRITERIA_H
