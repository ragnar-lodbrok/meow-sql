#include "query.h"
#include "exception.h"
#include <QDebug>

namespace meow {
namespace db {

Query::Query(Connection * connection)
    :_connection(connection)
{

}

Query::~Query()
{

}

void Query::setSQL(const QString & SQL)
{
    _SQL = SQL;
}

void Query::seekFirst()
{
    _curRecNo = -1;
    seekRecNo(0);
}

void Query::seekNext()
{
    seekRecNo(_curRecNo + 1);
}

QString Query::curRowColumn(const QString & colName, bool ignoreErrors /* = false */)
{
    std::size_t index;

    if (ignoreErrors) {
        try {
            index = indexOfColumn(colName);
        } catch(meow::db::Exception & ex) {
            qDebug() << "[Query] " << "Failed to find column by name: " << ex.message();
            return QString();
        }
    } else {
        index = indexOfColumn(colName);
    }

    return curRowColumn(index, ignoreErrors);
}

std::size_t Query::indexOfColumn(const QString & colName) const
{
    if (_columnIndexes.contains(colName)) {
        return _columnIndexes.value(colName);
    } else if (_columnIndexes.contains(colName.toLower())) {
        return _columnIndexes.value(colName.toLower());
    } else {
        throw db::Exception(
            QString("Column \"%1\" not available.").arg(colName)
        );
    }
}

} // namespace db
} // namespace meow

