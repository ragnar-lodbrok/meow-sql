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
     flags(0),
     dataTypeIndex(DataTypeIndex::None),
     dataTypeCategoryIndex(DataTypeCategoryIndex::Other)
    {

    }

    QString name;
    QString orgName;
    unsigned int flags;
    DataTypeIndex dataTypeIndex;
    DataTypeCategoryIndex dataTypeCategoryIndex; // cache
    // int nativeType;
};


} // namespace db
} // namespace meow

#endif // DB_QUERY_COLUMN_H
