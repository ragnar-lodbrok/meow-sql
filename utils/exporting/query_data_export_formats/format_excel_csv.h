#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_EXCEL_CSV_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_EXCEL_CSV_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatExcelCSV : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "Excel CSV";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_EXCEL_CSV_H
