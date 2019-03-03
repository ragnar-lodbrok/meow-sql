#ifndef DB_QUERY_COLUMN_H
#define DB_QUERY_COLUMN_H

#include "data_type/data_type.h"
#include <QString>

namespace meow {
namespace db {

struct QueryColumn
{
public:
    QueryColumn(const QString & columnName = QString())
    :name(columnName),
     orgName(columnName),
     flags(0)
    {

    }

    QString name;
    QString orgName;
    unsigned int flags;
    DataTypePtr dataType;
};


} // namespace db
} // namespace meow

#endif // DB_QUERY_COLUMN_H
