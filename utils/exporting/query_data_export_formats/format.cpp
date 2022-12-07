#include "format.h"
#include "ui/models/base_data_table_model.h"
#include <QCoreApplication>

namespace meow {
namespace utils {
namespace exporting {

QString QueryDataExportFormat::headerName(int col) const
{
    Q_ASSERT(_model);
    return _model->headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
}

QString QueryDataExportFormat::data(int row, int col) const
{
    Q_ASSERT(_model);
    return _model->data(_model->createIndexForRowCol(row, col),
                        Qt::DisplayRole).toString();
}

bool QueryDataExportFormat::isNull(int row, int col) const
{
    Q_ASSERT(_model);
    return _model->isNullAt(row, col);
}

bool QueryDataExportFormat::isNumericDataType(int col) const
{
    Q_ASSERT(_model);
    db::DataTypeCategoryIndex type = _model->typeCategoryForColumn(col);
    return type == db::DataTypeCategoryIndex::Integer
        || type == db::DataTypeCategoryIndex::Float;
}

QString QueryDataExportFormat::escapeEncloser(
        const QString & data,
        const QString & encloser) const
{
    const QString enc = encloser.isEmpty() ? this->encloser() : encloser;

    if (enc.isEmpty()) return data;

    QString res = data;
    res.replace(enc, enc + enc);
    return res;
}

QString QueryDataExportFormat::removeLineBreaks(const QString & data) const
{
    QString res = data;
    res.replace(QString("\r\n"), QChar(' '));
    res.replace(QChar('\r'), QChar(' '));
    res.replace(QChar('\n'), QChar(' '));
    return res;
}

QString QueryDataExportFormat::appNameWithVersion() const
{
    return qApp->applicationName() + ' ' + qApp->applicationVersion();
}

int QueryDataExportFormat::nextVisibleColumn(int curIndex) const {

    if (curIndex < -1) return -1;
    if (curIndex+1 > _model->columnCount()-1) return -1;

    ++curIndex; // result == next

    if (isIncludeAutoIncrementColumn() == false) {
        if (_model->columnIsAutoIncrement(curIndex)) {
            // skip auto incr column
            // recursion won't be deep for typical table or query
            return nextVisibleColumn(curIndex + 1);
        }
    }

    return curIndex;
}

int QueryDataExportFormat::totalColumnsCount() const {
    return _model->columnCount();
}

} // namespace exporting
} // namespace utils
} // namespace meow
