#ifndef DB_QUERY_DATA_EDITOR_H
#define DB_QUERY_DATA_EDITOR_H

#include <QStringList>

namespace meow {
namespace db {

class QueryData;

class QueryDataEditor
{
public:
    QueryDataEditor() {}
    virtual ~QueryDataEditor();

    bool applyModificationsInDB(QueryData * data);

    void deleteCurrentRow(QueryData * data);

protected:
    virtual void insert(QueryData * data,
                const QStringList & columns,
                const QStringList & values);
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_EDITOR_H
