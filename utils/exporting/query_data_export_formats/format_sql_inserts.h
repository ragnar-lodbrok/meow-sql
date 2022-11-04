#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatSQLInserts : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "sql_inserts";
    }

    virtual QString name() const override {
        return QObject::tr("SQL INSERTs");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H

