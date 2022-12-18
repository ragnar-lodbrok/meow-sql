#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatMarkdown : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "markdown";
    }

    virtual QString name() const override {
        return QObject::tr("Markdown Here");
    }

    virtual QString fileExtension() const override {
        return "md";
    }

    virtual QString header() const override {

        QString h;
        const QString LE = lineTerminator();

        h += sourceName() + LE;
        h += "---" + LE;

        if (isIncludeSQLQuery()) {
            h += "```sql" + LE;
            h += sqlQuery() + LE;
            h += "```" + LE;
        }

        // | col1 | col2 | col3 |
        h += "| ";

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            if (isIncludeColumnNames()) {
                QString colName = headerName(col);
                h += escMarkdown(colName);
            }
            h += " | ";
        }
        h += LE;

        if (rowsCount() == 0) {
            return h;
        }

        // | --- | --- | --- |
        h += "| ";

        col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            h += "---";
            if (isNumericDataType(col)) {
                h += ":";
            }
            h += " | ";
        }
        h += LE;

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();
        QString r = "| ";

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData;
            if (!_model->isNullAt(indexRow, col)) {
                colData = data(indexRow, col);

                if (!isNumericDataType(col)) {
                    if (isRemoveLineBreaksFromContents()) {
                        colData = removeLineBreaks(colData);
                    }
                    colData = escMarkdown(colData);
                }
            }

            r += colData + " | ";
        }

        r += LE;

        return r;
    }

    virtual OptionsValueMap defaultOptionsValue() const override {
        return {
            {OptionsValue::LineTerminator, QString("\r\n")}
        };
    }

    virtual OptionsValueSet editableOptionsValue() const override {
        return {
            OptionsValue::LineTerminator
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
    QString escMarkdown(const QString & str) const {

        QString res = str;

        res.replace(QLatin1Char('\\'), QLatin1String("\\\\")); // backslash
        res.replace(QLatin1Char('`'),  QLatin1String("\\`"));  // `
        res.replace(QLatin1Char('*'),  QLatin1String("\\*"));  // *
        res.replace(QLatin1Char('_'),  QLatin1String("\\_"));  // _
        res.replace(QLatin1Char('{'),  QLatin1String("\\{"));  // {
        res.replace(QLatin1Char('}'),  QLatin1String("\\}"));  // }
        res.replace(QLatin1Char('['),  QLatin1String("\\["));  // [
        res.replace(QLatin1Char(']'),  QLatin1String("\\]"));  // ]
        res.replace(QLatin1Char('('),  QLatin1String("\\("));  // (
        res.replace(QLatin1Char(')'),  QLatin1String("\\)"));  // )
        res.replace(QLatin1Char('#'),  QLatin1String("\\#"));  // #
        //res.replace(QLatin1Char('+'),  QLatin1String("\\+"));  // +
        //res.replace(QLatin1Char('-'),  QLatin1String("\\-"));  // -
        //res.replace(QLatin1Char('.'),  QLatin1String("\\."));  // .
        //res.replace(QLatin1Char('!'),  QLatin1String("\\!"));  // !
        res.replace(QLatin1Char('<'),  QLatin1String("&lt;")); // <
        res.replace(QLatin1Char('>'),  QLatin1String("&gt;")); // >

        return res;
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_MARKDOWN_H

