#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL

#include "format.h"
#include "ui/models/base_data_table_model.h"
#include "ui/models/data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExportFormatSQL : public QueryDataExportFormat
{
public:

    virtual QString fileExtension() const override {
        return "sql";
    }

    virtual QString row(int indexRow) const override {

        Q_ASSERT(_model);

        QString sql = sqlOperationIntoPart();

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
protected:

    virtual QString sqlOperation() const = 0;

    QString sqlQuoteId(const QString & str) const {
        return _model->connection()->quoteIdentifier(str);
    }

    QString sqlEscapeStr(const QString & str) const {
        return _model->connection()->escapeString(str);
    }

    QString sqlTableName() const {
        Q_ASSERT(_model);
        auto tableData = dynamic_cast<ui::models::DataTableModel *>(_model);
        if (tableData != nullptr) {
            return tableData->entity()->name();
        } else {
            return "UnknownTable";
        }
    }

    QString sqlColumnName(int col) const {
        return _model->queryData()->columnName(col);
    }

    QString sqlData(int row, int col) const {
        Q_ASSERT(_model);
        if (isNull(row, col)) {
            return "NULL";
        }

        QString data = sqlEscapeStr(this->data(row, col));

        // TODO: move db-specific formatting to specific
        // NativeQueryResult subclass?
        if (_model->connection()->connectionParams()->serverType()
                == db::ServerType::MySQL) {
            if (_model->queryData()->dataTypeForColumn(col)->index
                    == meow::db::DataTypeIndex::Bit) {
                return "b" + data;
            }
        }

        return data;
    }

    QString sqlWhereForRow(int row) const {
        return _model->queryData()->whereForRow(row);
    }

private:

    QString sqlOperationIntoPart() const
    {
        if (!_sqlOperationIntoPartCached.isEmpty()) {
            return _sqlOperationIntoPartCached;
        }

        QString sql = sqlOperation();
        sql += " INTO " + sqlQuoteId(sqlTableName());

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

        _sqlOperationIntoPartCached = sql;

        return sql;
    }

    mutable QString _sqlOperationIntoPartCached;
};


} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORT_FORMAT_SQL

