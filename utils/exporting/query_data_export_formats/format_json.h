#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H

#include "format.h"
#include "ui/models/base_data_table_model.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatJSON : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "json";
    }

    virtual QString name() const override {
        return QObject::tr("JSON");
    }

    virtual QString fileExtension() const override {
        return "json";
    }

    virtual QString header() const override {

        // clear
        _rootObject = QJsonObject();
        _rowsArray = QJsonArray();

        if (isIncludeSQLQuery()) {
            _rootObject.insert("query", sqlQuery());
        }

        _rootObject.insert("table", sourceName());

        _keyNamesCache.clear();
        _keyNamesCache.resize(totalColumnsCount());
        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            _keyNamesCache[col] = headerName(col);
        }

        return QString(); // serialize in footer()
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        QJsonObject rowObject;
        QJsonArray rowArray;
        bool useObject = isIncludeColumnNames();

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {

            QJsonValue jsonVal(QJsonValue::Null); // null

            if (!_model->isNullAt(indexRow, col)) {

                QString colData = data(indexRow, col);

                if (isNumericDataType(col)) {
                    jsonVal = QJsonValue(colData.toDouble()); // number
                } else {
                    if (isRemoveLineBreaksFromContents()) {
                        colData = removeLineBreaks(colData);
                    }
                    jsonVal = QJsonValue(colData); // string
                }
            }

            if (useObject) {
                QString keyName = _keyNamesCache[col];
                rowObject.insert(keyName, jsonVal);
            } else {
                rowArray << jsonVal;
            }
        }

        if (useObject) {
            _rowsArray << rowObject;
        } else {
            _rowsArray << rowArray;
        }

        return QString(); // serialize in footer()
    }

    virtual QString footer() const override {

        // TODO: this consumes a lot of RAM, use streaming

        _rootObject.insert("rows", _rowsArray);

        QJsonDocument doc(_rootObject);
        QByteArray data = doc.toJson(QJsonDocument::Indented);

        return QString(data);
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
    mutable QJsonObject _rootObject;
    mutable QJsonArray _rowsArray;
    mutable std::vector<QString> _keyNamesCache;
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_JSON_H

