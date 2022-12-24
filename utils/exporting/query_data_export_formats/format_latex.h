#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatLatex : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "latex";
    }

    virtual QString name() const override {
        return QObject::tr("LaTeX");
    }

    virtual QString fileExtension() const override {
        return "LaTeX";
    }

    virtual QString header() const override {

        QString h;
        const QString LE = lineTerminator();

        // \begin{tabular}{ c  c  c  c  c  c  c  c }
        h += "\\begin{tabular}";
        h += "{";
        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            h += " c ";
        }
        h += "}" + LE;

        // column names
        if (isIncludeColumnNames()) {
            QStringList colNames;
            col = -1;
            while ((col = nextVisibleColumn(col)) != -1) {
                QString colName = headerName(col);
                colNames << escLatex(colName);
            }
            h += colNames.join(" & ") + "\\\\ " + LE;
        }

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();

        QStringList colDatas;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData;
            if (!_model->isNullAt(indexRow, col)) {
                colData = data(indexRow, col);

                if (!isNumericDataType(col)) {
                    if (isRemoveLineBreaksFromContents()) {
                        colData = removeLineBreaks(colData);
                    }
                    colData = escLatex(colData);
                }
            }
            colDatas << colData;
        }

        return colDatas.join(" & ") + "\\\\ " + LE;
    }

    virtual QString footer() const override {

        QString f;
        const QString LE = lineTerminator();
        f += "\\end{tabular}" + LE;
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
    QString escLatex(const QString & str) const {

        // https://www.cespedes.org/blog/85/how-to-escape-latex-special-characters

        QString res;
        int strSize = str.size();

        for (int i = 0; i < strSize; ++i) {
            QChar ci = str.at(i);
            switch (ci.unicode()) {

            case QLatin1Char('\\').unicode():
                res += QLatin1String("\\textbackslash{}");
                break;

            case QLatin1Char('^').unicode():
                res += QLatin1String("\\textasciicircum{}");
                break;

            case QLatin1Char('~').unicode():
                res += QLatin1String("\\textasciitilde{}");
                break;

            case QLatin1Char('#').unicode():
            case QLatin1Char('$').unicode():
            case QLatin1Char('%').unicode():
            case QLatin1Char('&').unicode():
            case QLatin1Char('_').unicode():
            case QLatin1Char('{').unicode():
            case QLatin1Char('}').unicode():
                res += QLatin1Char('\\') + ci;
                break;

            default:
                res += ci;
                break;
            };
        }

        return res;
    }
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_LATEX_H

