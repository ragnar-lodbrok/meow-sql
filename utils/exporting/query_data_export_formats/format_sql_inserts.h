#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H

#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatSQLInserts : public QueryDataExportFormat
{
public:

    virtual QString id() const override {
        return "sql_inserts";
    }

    virtual QString name() const override {
        return QObject::tr("SQL INSERTs");
    }

    virtual QString fileExtension() const override {
        return "sql";
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        QString sql = sqlInsertIntoPart();

        QStringList colsData;

        int col = -1;
        while ((col = nextVisibleColumn(col)) != -1) {
            QString colData = sqlData(indexRow, col);
            colsData.push_back(colData);
        }

        sql += colsData.join(", ");
        sql += ");" + lineTerminator();

        return sql;
    }

    virtual OptionsValueMap defaultOptionsValue() const override {
        return {
            {OptionsValue::LineTerminator, QString("\r\n")},
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
    QString sqlInsertIntoPart() const
    {
        if (!_sqlInsertIntoPartCached.isEmpty()) {
            return _sqlInsertIntoPartCached;
        }

        QString sql;
        sql += "INSERT INTO " + sqlQuoteId(sqlTableName());

        if (isIncludeColumnNames()) {

            QStringList sqlColumnNames;

            int col = -1;
            while ((col = nextVisibleColumn(col)) != -1) {
                QString colData = sqlQuoteId(sqlColumnName(col));
                sqlColumnNames.push_back(colData);
            }

            sql += " (" + sqlColumnNames.join(", ") + ")";
        }

        sql += " VALUES (";

        _sqlInsertIntoPartCached = sql;

        return sql;
    }

    mutable QString _sqlInsertIntoPartCached;
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL_INSERTS_H

