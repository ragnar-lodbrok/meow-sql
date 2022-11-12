#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

#include "format.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatHTMLTable : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "html_table";
    }

    virtual QString name() const override {
        return QObject::tr("HTML table");
    }

    virtual QString fileExtension() const override {
        return "html";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

