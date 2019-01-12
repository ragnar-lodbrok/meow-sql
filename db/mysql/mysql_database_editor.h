#ifndef DB_MYSQL_DATABASE_EDITOR_H
#define DB_MYSQL_DATABASE_EDITOR_H

#include "db/database_editor.h"

namespace meow {
namespace db {

class MySQLDataBaseEditor : public DataBaseEditor
{
public:
    MySQLDataBaseEditor(Connection * connection);
    virtual void create(const QString & name,
                        const QString & collation = QString()) override;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_DATABASE_EDITOR_H
