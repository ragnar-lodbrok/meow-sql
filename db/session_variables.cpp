#include "session_variables.h"
#include "connection.h"
#include <QDebug>

namespace meow {
namespace db {

SessionVariables::SessionVariables(Connection * connection)
    : _connection(connection)
    , _fetched(false)
{

}

SessionVariables::~SessionVariables()
{

}

void SessionVariables::fetch(bool refresh)
{
    if (_fetched && !refresh) {
        return;
    }

    _fetched = true;

    _sessionVars = fetchForSession();
    _globalVars = fetchGlobal();

    // make unique list of all names form 2 maps name => value
    _allNames = _sessionVars.keys();

    auto it = _globalVars.constBegin();
    while (it != _globalVars.constEnd()) {
        const QString & varName = it.key();
        if (!_sessionVars.contains(varName)) { // check map not list
            _allNames.append(varName);
        }
        ++it;
    }
}

int SessionVariables::totalCount()
{
    return _allNames.count();
}

QString SessionVariables::value(const QString & name, bool global) const
{
    if (global) {
        return _globalVars.value(name, QString());
    } else {
        return _sessionVars.value(name, QString());
    }
}

// ---------------------------------------------------------------

MySQLSessionVariables::MySQLSessionVariables(Connection * connection)
    : SessionVariables(connection)
{

}


VarsMap MySQLSessionVariables::fetchForSession()
{
    return fetchWithQuery("SHOW VARIABLES");
}

VarsMap MySQLSessionVariables::fetchGlobal()
{
    return fetchWithQuery("SHOW GLOBAL VARIABLES");
}

VarsMap MySQLSessionVariables::fetchWithQuery(const QString & SQL)
{
    VarsMap vars;

    QList<QStringList> rows = _connection->getRows(SQL);

    for (const QStringList & var : rows) {
        vars[var[0]] = var[1];
    }

    return vars;
}

} // namespace db
} // namespace meow
