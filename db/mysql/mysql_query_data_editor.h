#ifndef DB_MYSQL_QUERY_DATA_EDITOR_H
#define DB_MYSQL_QUERY_DATA_EDITOR_H

#include "db/query_data_editor.h"

namespace meow {
namespace db {

class MySQLQueryDataEditor : public QueryDataEditor
{

protected:
    virtual void insert(QueryData * data,
                const QStringList & columns,
                const QStringList & values) override;

};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_QUERY_DATA_EDITOR_H
