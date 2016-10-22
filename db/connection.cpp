#include "connection.h"
#include "query.h"

namespace meow {
namespace db {

Connection::Connection(const ConnectionParameters & params)
    :_active(false),
     _connectionParams(params),
     _characterSet(),
     _isUnicode(false)
{

}

Connection::~Connection()
{

}

void Connection::doBeforeConnect()
{
    // TODO
    // H:
}

void Connection::doAfterConnect()
{
    // TODO
    // H:
}

QStringList Connection::allDatabases()
{
    return fetchDatabases();
}

// virtual
void Connection::setCharacterSet(const QString & characterSet)
{
   _characterSet = characterSet;
}

} // namespace db
} // namespace meow
