#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatSQLDeletesReplaces : public IQueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "sql_deletes_inserts";
    }

    virtual QString name() const override {
        return QObject::tr("SQL DELETEs/INSERTs");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H

