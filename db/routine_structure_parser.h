#ifndef DB_ROUTINE_STRUCTURE_PARSER_H
#define DB_ROUTINE_STRUCTURE_PARSER_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class RoutineEntity;

class RoutineStructureParser
{
public:
    explicit RoutineStructureParser(Connection * connection);
    void run(RoutineEntity * routine);
private:
    QString extractLiteral(QString & SQL, const QString & prefix = QString());
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_ROUTINE_STRUCTURE_PARSER_H
