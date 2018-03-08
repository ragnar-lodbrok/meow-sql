#ifndef DATABASE_TABLE_EDITOR_H
#define DATABASE_TABLE_EDITOR_H


namespace meow {
namespace db {

class Connection;
class TableEntity;

class TableEditor
{
public:
    TableEditor(Connection * connection);
    virtual ~TableEditor() {}
    virtual bool edit(TableEntity * table, TableEntity * newData) = 0;
    virtual bool insert(TableEntity * table) = 0;
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DATABASE_TABLE_EDITOR_H
