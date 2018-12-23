#ifndef DATABASE_EDITOR_H
#define DATABASE_EDITOR_H

#include <QString>

namespace meow {
namespace db {

class Connection;

class DataBaseEditor
{
public:
    DataBaseEditor(Connection * connection);
    virtual ~DataBaseEditor();

    virtual bool drop(const QString & databaseName);
    // TODO: create
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DATABASE_EDITOR_H
