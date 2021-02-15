#ifndef DB_SESSION_VARIABLES_H
#define DB_SESSION_VARIABLES_H

#include <QMap>
#include "data_type/data_type_category.h"

namespace meow {
namespace db {

class Connection;

using VarsMap = QMap<QString, QString>; // name => value

// TODO rename to Variables/ConnectionVariables?
class SessionVariables
{
public:
    explicit SessionVariables(Connection * connection);
    virtual ~SessionVariables();
    virtual VarsMap fetchForSession() = 0;
    virtual VarsMap fetchGlobal() = 0;
    virtual DataTypeCategoryIndex dataType(const QString & name) const;

    virtual bool editVariableInDB(const QString & name,
                                  const QString & value,
                                  bool global) = 0;

    void fetch(bool refresh = false);

    int totalCount();

    QString value(const QString & name, bool global = false) const;

    bool sessionAndGlobalDiffers(const QString & name) const {
        return value(name, false) != value(name, true);
    }

    const QStringList & allNames() const {
        return _allNames;
    }

protected:
    Connection * _connection;
private:
    QStringList _allNames;
    VarsMap _sessionVars;
    VarsMap _globalVars;
    bool _fetched;
};

// TODO: move to separate file
class MySQLSessionVariables : public SessionVariables
{
public:

    explicit MySQLSessionVariables(Connection * connection);

    virtual VarsMap fetchForSession() override;
    virtual VarsMap fetchGlobal() override;
    virtual bool editVariableInDB(const QString & name,
                                  const QString & value,
                                  bool global) override; // throws
private:
    VarsMap fetchWithQuery(const QString & SQL);
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_VARIABLES_H
