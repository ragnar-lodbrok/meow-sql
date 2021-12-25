#ifndef DB_CONNECTION_QUERY_KILLER_H
#define DB_CONNECTION_QUERY_KILLER_H

#include <memory>
#include <QString>

namespace meow {
namespace db {

class Connection;

class ConnectionQueryKiller
{
public:
    explicit ConnectionQueryKiller(Connection * connection);
    virtual ~ConnectionQueryKiller();

    virtual void run();
protected:

    virtual QString killQueryStatement() const;

    Connection * _connection;
};

using ConnectionQueryKillerPtr = std::shared_ptr<ConnectionQueryKiller>;

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_QUERY_KILLER_H
