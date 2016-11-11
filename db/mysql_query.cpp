#include <QDebug>
#include "mysql_query.h"

namespace meow {
namespace db {

MySQLQuery::MySQLQuery(Connection *connection)
    :Query(connection)
{

}

void MySQLQuery::execute(bool addResult /*= false*/, int useRawResult /*= -1*/) // override
{
    qDebug() << "[MySQLQuery] " << "Executing: " << SQL();

    // Execute a query, or just take over one of the last result pointers
    if (useRawResult == -1) {
        connection()->query(this->SQL(), true); //H: FStoreResult seems always true?
        useRawResult = 0;
    }

    MySQLResult lastResult = nullptr;
    if (connection()->lastResultsCount() > useRawResult) {
        lastResult = static_cast<MySQLConnection *>(connection())->lastRawResultAt(useRawResult);
    }

    if (addResult) {
        throw std::runtime_error("not implemented");
    } else {
        _resultList.clear();
        _recordCount = 0;
        // H: ...
    }

    if (lastResult) {
        _resultList.push_back(lastResult);
        _recordCount += lastResult.get()->row_count;
    }

    if (!addResult) {
        if (_resultList.empty() == false) {
            // FCurrentResults is normally done in SetRecNo, but never if result has no rows
            // H: FCurrentResults := LastResult;

            unsigned int numFields = mysql_num_fields(lastResult.get());

            //H: SetLength(FColumnTypes, NumFields);
            //H: SetLength(FColumnLengths, NumFields);
            //H: SetLength(FColumnFlags, NumFields);

            _columnNames.clear(); // TODO: try reserve() ?
            _columnOrgNames.clear();
            for (unsigned int i=0; i < numFields; ++i) {
                MYSQL_FIELD * field = mysql_fetch_field_direct(lastResult.get(), i);
                _columnNames.append(QString(field->name));
                if (connection()->serverVersionInt() >= 40100) {
                    _columnOrgNames.append(QString(field->org_name));
                } else {
                    _columnOrgNames.append(QString(field->name));
                }
            }
        }
    }
}

bool MySQLQuery::hasResult() // override
{
    return _resultList.empty() == false;
}

MySQLQuery::~MySQLQuery()
{

}

} // namespace db
} // namespace meow

