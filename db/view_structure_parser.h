#ifndef DB_VIEW_STRUCTURE_PARSER_H
#define DB_VIEW_STRUCTURE_PARSER_H

#include <QRegularExpression>

namespace meow {
namespace db {

class Connection;
class ViewEntity;

// TODO: different class per DB type
class ViewStructureParser
{
public:
    explicit ViewStructureParser(Connection * connection);
    void run(ViewEntity * view);
private:
    Connection * _connection;
    QRegularExpression _regexp;
};

} // namespace db
} // namespace meow

#endif // DB_VIEW_STRUCTURE_PARSER_H
