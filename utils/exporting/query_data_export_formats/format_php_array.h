#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatPHPArray : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "PHP Array";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

