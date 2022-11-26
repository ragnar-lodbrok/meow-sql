#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H

#include "format_sql_inserts.h"

namespace meow {
namespace utils {
namespace exporting {

// Inherit from INSERT, prefix with DELETE
class QueryDataExportFormatSQLDeletesReplaces
        : public QueryDataExportFormatSQLInserts
{
public:

    virtual QString id() const override {
        return "sql_deletes_inserts";
    }

    virtual QString name() const override {
        return QObject::tr("SQL DELETEs/INSERTs");
    }

    virtual QString row(int indexRow) const override {

        QString deleteSql = "DELETE FROM ";
        deleteSql += sqlQuoteId(sqlTableName());
        deleteSql += " WHERE " + sqlWhereForRow(indexRow);
        deleteSql += ";" + lineTerminator();

        return deleteSql
            + QueryDataExportFormatSQLInserts::row(indexRow);
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_DEL_REPLACES_H

