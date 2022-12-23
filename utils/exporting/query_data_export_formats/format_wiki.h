#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatWiki : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "wiki";
    }

    virtual QString name() const override {
        return QObject::tr("Wiki markup");
    }

    virtual QString fileExtension() const override {
        return "wiki";
    }

    virtual QString header() const override {

        QString h;
        const QString LE = lineTerminator();

        if (isIncludeColumnNames()) {

            h += "||";

            int col = -1;
            while ((col = nextVisibleColumn(col)) != -1) {
                QString colName = headerName(col);
                colName = '*' + escWiki(colName) + '*';
                h += ' ';
                h += colName;
                h += " ||";
            }
            h += LE;
        }

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();
        QString r = "|| ";

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData;
            if (!_model->isNullAt(indexRow, col)) {
                colData = data(indexRow, col);

                if (!isNumericDataType(col)) {
                    if (isRemoveLineBreaksFromContents()) {
                        colData = removeLineBreaks(colData);
                    }
                    colData = escWiki(colData);
                }
            }

            r += colData + " || ";
        }

        r = r.trimmed();

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
            OptionsBool::IncludeAutoIncrementColumn
        };
    }

    virtual OptionsBoolSet editableOptionsBool() const override {
        return {
            OptionsBool::IncludeColumnNames,
            OptionsBool::IncludeAutoIncrementColumn,
            OptionsBool::RemoveLineBreaksFromContents
        };
    }
private:
    QString escWiki(const QString & str) const {
        return str; // TODO?
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_WIKI_H

