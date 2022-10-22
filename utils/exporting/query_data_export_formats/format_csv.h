#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatCSV : public IQueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "csv";
    }

    virtual QString name() const override {
        return QObject::tr("CSV");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H
