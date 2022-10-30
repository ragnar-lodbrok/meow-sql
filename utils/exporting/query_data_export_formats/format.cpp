#include "format.h"
#include "ui/models/base_data_table_model.h"

namespace meow {
namespace utils {
namespace exporting {

QString IQueryDataExportFormat::data(int row, int col) const
{
    Q_ASSERT(_model);
    return _model->data(_model->createIndexForRowCol(row, col),
                        Qt::DisplayRole).toString();
}

bool IQueryDataExportFormat::isNull(int row, int col) const
{
    Q_ASSERT(_model);
    return _model->isNullAt(row, col);
}

bool IQueryDataExportFormat::isNumericDataType(int col) const
{
    Q_ASSERT(_model);
    db::DataTypeCategoryIndex type = _model->typeCategoryForColumn(col);
    return type == db::DataTypeCategoryIndex::Integer
        || type == db::DataTypeCategoryIndex::Float;
}

QString IQueryDataExportFormat::escapeEncloser(const QString & data) const
{
    if (encloser().isEmpty()) return data;

    QString res = data;
    res.replace(encloser(), encloser() + encloser());
    return res;
}

} // namespace exporting
} // namespace utils
} // namespace meow
