#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_FACTORY_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_FACTORY_H

#include <vector>
#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatFactory
{
public:
    std::vector<QueryDataExportFormatPtr> createFormats() const;
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_FACTORY_H
