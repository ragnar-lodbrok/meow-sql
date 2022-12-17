#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatPHPArray : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "php_array";
    }

    virtual QString name() const override {
        return QObject::tr("PHP Array");
    }

    virtual QString fileExtension() const override {
        return "php";
    }

    virtual QString header() const override {

        QString h;
        const QString LE = lineTerminator();

        if (outputfile()) {
            h += "<?php" + LE;
        }

        h += "$" + sourceName() + " = array(" + LE;

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();
        QString r = "\tarray(" + LE;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData;
            if (_model->isNullAt(indexRow, col)) {
                colData = "NULL";
            } else {
                colData = data(indexRow, col);

                if (!isNumericDataType(col)) {
                    if (isRemoveLineBreaksFromContents()) {
                        colData = removeLineBreaks(colData);
                    }
                    colData = escPHPString(colData);
                }
            }

            r += "\t\t";
            if (isIncludeColumnNames()) {
                QString colName = headerName(col);
                r += escPHPString(colName);
                r += " => ";
            }
            r += colData + ',' + LE;
        }

        r += "\t)," + LE;

        return r;
    }

    virtual QString footer() const override {

        QString f;
        const QString LE = lineTerminator();

        f += ");" + LE;

        if (outputfile()) {
            f += "?>";
        }

        return f;
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
            OptionsBool::RemoveLineBreaksFromContents
        };
    }
private:
    QString escPHPString(const QString & str) const {

        // Fun with C++: if you replace "double slash" with 2 slash symbols
        // next line won't work!

        QString res = str;

        res.replace(QLatin1String("\\"),
                    QLatin1String("\\\\"));  // \ => double slash
        res.replace(QLatin1Char('"'),  QLatin1String("\\\""));    // " => \"
        res.replace(QLatin1Char('\r'), QLatin1String("\\r"));     // \r => \\r
        res.replace(QLatin1Char('\n'), QLatin1String("\\n"));     // \n => \\n
        res.replace(QLatin1Char('\t'), QLatin1String("\\t"));     // \t => \\t

        return '"' + res + '"';
    }

};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_PHP_ARRAY_H

