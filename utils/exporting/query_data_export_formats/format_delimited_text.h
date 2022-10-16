#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatDelimitedText : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "Delimited text";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_DELIMITED_TEXT_CSV_H
