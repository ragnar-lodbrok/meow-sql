#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatCSV : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "csv";
    }

    virtual QString name() const override {
        return QObject::tr("CSV");
    }

    virtual QString fileExtension() const override {
        return "csv";
    }

    virtual QString header() const override {

        if (!isIncludeColumnNames()) {
            return QString();
        }

        QStringList colsData;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData = headerName(col);
            colData = encloser()
                    + escapeEncloser(colData)
                    + encloser();
            colsData.push_back(colData);
        }

        return colsData.join(fieldSeparator()) + lineTerminator();
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        QStringList colsData;

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

                QString encloser = this->encloser();
                if (encloser.isEmpty() && colData.contains(fieldSeparator())) {
                    // if e.g. fieldSeparator is [,] we need to force ["] when
                    // data contains [,]
                    encloser = "\"";
                }

                colData = encloser
                        + escapeEncloser(colData, encloser)
                        + encloser;
            }
            colsData.push_back(colData);
        }

        return colsData.join(fieldSeparator()) + lineTerminator();
    }

    virtual OptionsValueMap defaultOptionsValue() const override {
        return {
            {OptionsValue::FieldSeparator, QString(",")},
            {OptionsValue::Encloser,       QString('"')},
            {OptionsValue::LineTerminator, QString("\r\n")},
            {OptionsValue::NullValue,      QString()}
        };
    }

    virtual OptionsValueSet editableOptionsValue() const override {
        return {
            OptionsValue::FieldSeparator,
            OptionsValue::Encloser,
            OptionsValue::LineTerminator,
            OptionsValue::NullValue
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
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_CSV_H
