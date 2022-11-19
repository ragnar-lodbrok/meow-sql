#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

#include "format.h"
#include <QDateTime>
#include "ui/models/base_data_table_model.h"

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
        const QString LE = lineTerminator();

        h += "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"";
        h += LE;

        h += "  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">";
        h += LE + LE;

        h += "<html>" + LE;
        h += "  <head>" + LE;
        h += "    <title>" + escHTML(sourceName()) + "</title>" + LE;
        h += "    <meta name=\"GENERATOR\" content=\"" +
               escHTML(appNameWithVersion())  + "\">" + LE;

        QString encoding = this->encoding();
        encoding = encoding.toLower();
        if (encoding.isEmpty())  {
            encoding = "utf-8";
        }

        h += "    <meta http-equiv=\"Content-Type\"";
        h += " content=\"text/html; charset=" + escHTML(encoding) +
           "\" />" + LE;

        h += "    <style type=\"text/css\">" + LE;
        h += "      thead tr {background-color: ActiveCaption;";
        h += " color: CaptionText;}" + LE;
        h += "thead th {border: 1px solid silver;}" + LE;
        h += "      th, td {vertical-align: top;";
        h += " font-family: Tahoma, Times, Arial, Helvetica, sans-serif;";
        h += " font-size: 8pt; padding: 7px; }" + LE;
        h += "      table, td {border: 1px solid silver;}" + LE;
        h += "      table {border-collapse: collapse;}" + LE;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            int colWidth = columnWidth(col);
            if (colWidth) {
                h += "      thead .col" + QString::number(col);
                h += " {width: " + QString::number(colWidth) + "px;}" + LE;
            }
        }

        h += "    </style>" + LE;
        h += "  </head>" + LE;
        h += "  <body>" + LE;

        if (isIncludeSQLQuery()) {
            h += "    <p style=\"font-family: monospace; white-space: pre;\">";
            h += escHTML(sqlQuery()) + "</p>" + LE;
        }

        h += "    <table caption=\"" + escHTML(sourceName());
        h += " (" + QString::number(rowsCount()) + " rows)\">" + LE;

        if (isIncludeColumnNames()) {
            h += "      <thead>" + LE;
            h += "         <tr>" + LE;

            int col = -1;
            while ((col = nextVisibleColumn(col)) != -1) {
                QString colName = headerName(col);
                h += "          <th class=\"col" + QString::number(col) + "\">";
                h += escHTML(colName) + "</th>" + LE;
            }

            h += "         </tr>" + LE;
            h += "      </thead>" + LE;
        }

        h += "      <tbody>" + LE;

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();
        QString r = "        <tr>" + LE;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData;
            if (_model->isNullAt(indexRow, col)) {
                colData = nullValue();
            } else {
                colData = data(indexRow, col);

                if (isRemoveLineBreaksFromContents()) {
                    colData = removeLineBreaks(colData);
                }
            }

            colData = escHTML(colData);
            r += "          <td>";
            r += colData + "</td>" + LE;

        }

        r += "        </tr>" + LE;

        return r;
    }

    virtual QString footer() const override {

        QString f;
        const QString LE = lineTerminator();

        f += "      </tbody>" + LE;
        f += "    </table>" + LE;
        f += "    <p>" + LE;
        f += "      <em>Generated " + dateTimeNowString();
        f += " by <a href=\"" + appURL() + "\">";
        f += escHTML(appNameWithVersion()) + "</a></em>" + LE;
        f += "    </p>" + LE + LE;
        f += "  </body>" + LE;
        f += "</html>" + LE;

        return f;
    }

    virtual OptionsValueMap defaultOptionsValue() const override {
        return {
            {OptionsValue::LineTerminator, QString("\r\n")},
            {OptionsValue::NullValue,      QString()}
        };
    }

    virtual OptionsValueSet editableOptionsValue() const override {
        return {
            OptionsValue::LineTerminator,
            OptionsValue::NullValue
        };
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
private:
    QString dateTimeNowString() const {
         return QDateTime::currentDateTime().toString(Qt::ISODate);
    }
    QString appURL() const {
        return "https://github.com/ragnar-lodbrok/meow-sql";
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_HTML_TABLE_H

