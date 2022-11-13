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

    virtual QString header() const override {

        QString h;
        const QString LE = "\r\n";

        h += "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"";
        h += LE;

        h += "  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">'";
        h += LE + LE;

        h += "<html>" + LE;
        h += "  <head>" + LE;
        h += "    <title>" + escHTML(sourceName()) + "</title>";
        //h += "    <meta name=\"GENERATOR\" content=\""
        //         + "\">";

        return h;
    }

    virtual OptionsBoolSet defaultOptionsBool() const override {
        return {
            OptionsBool::IncludeColumnNames,
            OptionsBool::IncludeAutoIncrementColumn,
            OptionsBool::IncludeSQLQuery
        };
    }

    virtual OptionsBoolSet editableOptionsBool() const override {
        return {
            OptionsBool::IncludeColumnNames,
            OptionsBool::IncludeAutoIncrementColumn,
            OptionsBool::IncludeSQLQuery,
            OptionsBool::RemoveLineBreaksFromContents
        };
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

