#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatWiki : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "Wiki markup";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

