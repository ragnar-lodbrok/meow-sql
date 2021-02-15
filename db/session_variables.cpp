#include "session_variables.h"
#include "connection.h"
#include <QRegularExpression>
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

DataTypeCategoryIndex SessionVariables::dataType(const QString & name) const
{
    QString value;
    if (_sessionVars.contains(name)) {
        value = _sessionVars.value(name);
    } else {
        value = _globalVars.value(name);
    }

    {
        bool isInt = false;
        value.toInt(&isInt);
        if (isInt) {
            return DataTypeCategoryIndex::Integer;
        }
    }

    {
        bool isDouble = false;
        value.toDouble(&isDouble);
        if (isDouble) {
            return DataTypeCategoryIndex::Float;
        }
    }

    if (value.length() == 2 || value.length() == 3) { // ON/OFF YES/NO len
        value = value.toUpper();
        if (value == "ON" || value == "OFF" ||
                value == "YES" || value == "NO") {
            return DataTypeCategoryIndex::Other;
        }
    }

    return DataTypeCategoryIndex::Text;
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
    Q_ASSERT(_fetched == true); // TODO: fetch if not
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

bool MySQLSessionVariables::editVariableInDB(const QString & name,
                                             const QString & value,
                                             bool global)
{
    QString oldValue = this->value(name, global);

    if (oldValue == value) {
        return false;
    }

    QRegularExpression numericRegExp(R"(^\d+(\.\d*)?$)"); // ^\d+(\.\d*)?$

    bool isNumeric = numericRegExp.match(value).hasMatch();

    QString SQL = QString("SET @@%1.%2 = %3")
            .arg(global ? "GLOBAL" : "SESSION")
            .arg(name) // from trusted source
            .arg(isNumeric ? value : _connection->escapeString(value));

    _connection->query(SQL);

    return true;
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
