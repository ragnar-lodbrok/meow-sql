#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatXML : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "xml";
    }

    virtual QString name() const override {
        return QObject::tr("XML");
    }

    virtual QString fileExtension() const override {
        return "xml";
    }

    virtual QString header() const override {

        // TODO: use QXmlStreamWriter?

        QString h;
        const QString LE = lineTerminator();

        QString encoding = this->encoding();
        encoding = encoding.toLower();
        if (encoding.isEmpty())  {
            encoding = "utf-8";
        }

        h += "<?xml version=\"1.0\" encoding=\"";
        h += escXMLText(encoding) + "\"?>" + LE + LE;

        if (isIncludeSQLQuery()) {
            h += "<resultset statement=\"";
            h += escXMLText(sqlQuery());
            h += "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
            h += LE;
        } else {
            h += "<table_data name=\"";
            h += escXMLText(sourceName());
            h += "\">" + LE;
        }

        _colNamesCache.clear();
        _colNamesCache.resize(totalColumnsCount());
        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            _colNamesCache[col] = escXMLText(headerName(col));
        }

        return h;
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        const QString LE = lineTerminator();

        QString r;
        r += "\t<row>" + LE;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString field;
            field += "\t\t<field";
            if (isIncludeColumnNames()) {
                QString name = _colNamesCache[col];
                field += " name=\"" + name + '"';
            }
            if (_model->isNullAt(indexRow, col)) {
                field += " xsi:nil=\"true\" />" + LE;
            } else {
                QString colData = data(indexRow, col);
                field += ">" + escXMLText(colData);
                field += "</field>" + LE;
            }

            r += field;
        }
        r += "\t</row>" + LE;

        return r;
    }

    virtual QString footer() const override {
        QString f;
        const QString LE = lineTerminator();

        if (isIncludeSQLQuery()) {
            f += "</resultset>" + LE;
        } else {
            f += "</table_data>" + LE;
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
            OptionsBool::IncludeSQLQuery,
            OptionsBool::RemoveLineBreaksFromContents
        };
    }
private:
    QString escXMLText(const QString & str) const {
        QString res = str;
        res.replace(QChar('&'),  QLatin1String("&amp;"));    // & - keep first
        res.replace(QChar('"'),  QLatin1String("&quot;"));   // "
        res.replace(QChar('\''), QLatin1String("&apos;"));   // '
        res.replace(QChar('<'),  QLatin1String("&lt;"));     // <
        res.replace(QChar('>'),  QLatin1String("&gt;"));     // >
        // TODO: control codes
        return res;
    }

    mutable std::vector<QString> _colNamesCache;
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_XML_H

