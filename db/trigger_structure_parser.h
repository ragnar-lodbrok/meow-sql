#ifndef DB_TRIGGER_STRUCTURE_PARSER_H
#define DB_TRIGGER_STRUCTURE_PARSER_H

#include <QRegularExpression>

namespace meow {
namespace db {

class Connection;
class TriggerEntity;

class TriggerStructureParser
{
public:
    explicit TriggerStructureParser(Connection * connection);
    void run(TriggerEntity * trigger);
private:
    Connection * _connection;
    QRegularExpression _regexp;
};

} // namespace db
} // namespace meow


#endif // DB_TRIGGER_STRUCTURE_PARSER_H
