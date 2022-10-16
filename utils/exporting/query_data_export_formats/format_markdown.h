#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H

#include <QString>
#include <memory>

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatMarkdown : public IQueryDataExportFormat
{
public:
    virtual QString name() const override {
        return "Markdown Here";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H

