#ifndef DB_QUERY_DATA_EDITOR_H
#define DB_QUERY_DATA_EDITOR_H

namespace meow {
namespace db {

class QueryData;

class QueryDataEditor
{
public:
    QueryDataEditor() {}

    void applyModificationsInDB(QueryData * data);
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_EDITOR_H
