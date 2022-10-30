#include "base_data_table_model.h"
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

BaseDataTableModel::BaseDataTableModel(meow::db::QueryDataPtr queryData,
                                         QObject *parent)
    : QAbstractTableModel(parent),
      _queryData(queryData),
      _rowCount(0),
      _colCount(0),
      _sortFilterModel(nullptr),
      _filterPatternIsRegexp(false)
{

}

bool BaseDataTableModel::isNullAt(int row, int column) const
{
    return _queryData->isNullAt(row, column);
}

void BaseDataTableModel::setRowCount(int newRowCount)
{
    _rowCount = newRowCount;
}

void BaseDataTableModel::setColumnCount(int newColumnCount)
{
    _colCount = newColumnCount;
}

int BaseDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_colCount != -1) {
        return _colCount;
    }
    return _queryData->columnCount();
}

int BaseDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_rowCount != -1) {
        return _rowCount;
    }
    return _queryData->rowCount();
}

Qt::ItemFlags BaseDataTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant BaseDataTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    switch (role) {

    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal) {
            return _queryData->columnName(section);
        }
        break;

    case Qt::DecorationRole:
        // TODO: cache icons?
        if (_queryData->columnIsPrimaryKeyPart(section)) {
            return QIcon(":/icons/key_primary.png");
        } else if (_queryData->columnIsUniqueKeyPart(section)) {
            return QIcon(":/icons/key_unique.png");
        } else if (_queryData->columnIsIndexKeyPart(section)) {
            return QIcon(":/icons/key_index.png");
        }
        break;

    default:
        break;
    }


    return QVariant();
}

QVariant BaseDataTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {


    case Qt::EditRole: {
        // TODO: EnsureFullRow
        _queryData->prepareEditing(); // Let's do it here
        return _queryData->editDataAt(index.row(), index.column());
    }
    case Qt::DisplayRole:        
        return _queryData->displayDataAt(index.row(), index.column());

    case Qt::ForegroundRole: {
        auto textSettings = meow::app()->settings()->textSettings();
        auto dataType = _queryData->columnDataTypeCategory(index.column());
        if (dataType != meow::db::DataTypeCategoryIndex::None) {
            bool isNull = _queryData->isNullAt(index.row(), index.column());
            if (isNull == false) {
                return textSettings->colorForDataType(dataType);
            } else {
                return textSettings->colorForDataTypeNULL(dataType);
            }
        }
        return QVariant();
    }

    default:
        return QVariant();
    }

}

QAbstractItemModel * BaseDataTableModel::createSortFilterModel()
{
    if (_sortFilterModel == nullptr) {
        _sortFilterModel = new QueryDataSortFilterProxyModel(queryData(), this);
        _sortFilterModel->setSourceModel(this);
        _sortFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _sortFilterModel->setFilterKeyColumn(-1); // all columns

        setFilterPattern(_filterPattern, _filterPatternIsRegexp);
    }
    return _sortFilterModel;
}

void BaseDataTableModel::setFilterPattern(const QString & pattern, bool regexp)
{
    if (_filterPattern == pattern && _filterPatternIsRegexp == regexp) return;

    _filterPattern = pattern;
    _filterPatternIsRegexp = regexp;
    if (_sortFilterModel) {
        if (_filterPatternIsRegexp) {
            _sortFilterModel->setFilterRegExp(QRegExp(pattern,
                                                      Qt::CaseInsensitive,
                                                      QRegExp::RegExp));
        } else {
            _sortFilterModel->setFilterWildcard(pattern);
        }
    }
}

QString BaseDataTableModel::filterPattern() const
{
    return _filterPattern;
}

int BaseDataTableModel::filterMatchedRowCount() const
{
    if (_sortFilterModel) {
        return _sortFilterModel->rowCount();
    } else {
        return rowCount(); // all matched if no filter
    }
}

} // namespace models
} // namespace ui
} // namespace meow
