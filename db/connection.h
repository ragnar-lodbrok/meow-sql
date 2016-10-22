#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <QString>
#include <QStringList>
#include "connection_parameters.h"
#include "exception.h"

namespace meow {
namespace db {

class Query;

class Connection
{
public:
    Connection(const ConnectionParameters & params);
    ~Connection();

    bool active() const { return _active; }
    ConnectionParameters * connectionParams() { return &_connectionParams; }
    QString & characterSet() { return _characterSet; }
    bool isUnicode() const { return _isUnicode; }

    QStringList allDatabases();
    virtual QStringList fetchDatabases() = 0;
    virtual Query * createQuery() const = 0;
    virtual void setActive(bool active) = 0;
    virtual bool ping(bool reconnect) = 0;
    virtual QString getLastError() = 0;
    virtual void doBeforeConnect();
    virtual void doAfterConnect();
    virtual QString fetchCharacterSet() = 0;
    virtual void setCharacterSet(const QString & characterSet);
    void setIsUnicode(bool isUnicode) { _isUnicode = isUnicode; }
protected:
    //QStringList getColumn(const QString & SQL, int index = 0); // H: GetCol
    bool _active;
private:
    //int _connectionStarted;
    //int _serverUptime;
    ConnectionParameters _connectionParams;
    QString _characterSet;
    bool _isUnicode;
    //bool _loginPromptDone;
    //QString _databaseName;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_H
