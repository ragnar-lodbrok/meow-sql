#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

#include "format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatHTMLTable : public IQueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "html_table";
    }

    virtual QString name() const override {
        return QObject::tr("HTML table");
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

