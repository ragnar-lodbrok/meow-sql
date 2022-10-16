#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatJSON : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "JSON";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H

