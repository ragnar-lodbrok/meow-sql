#ifndef DATABASE_EDITOR_H
#define DATABASE_EDITOR_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class DataBaseEntity;

class DataBaseEditor
{
public:
    DataBaseEditor(Connection * connection);
    virtual ~DataBaseEditor();

    virtual bool drop(const QString & databaseName);
    virtual void create(const QString & name,
                        const QString & collation = QString());
    virtual bool edit(DataBaseEntity * database,
                      const QString & newName,
                      const QString & newCollation = QString())
    {
        return false; // not implemented in base, see ancestors
    }

protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DATABASE_EDITOR_H
