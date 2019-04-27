#include "table_columns_model.h"
#include "db/entity/table_entity.h"
#include <QDebug>
#include <QFont>
#include "app/app.h"
#include "db/connection.h"

namespace meow {
namespace models {
namespace db {

TableColumnsModel::TableColumnsModel(QObject * parent)
    : QAbstractTableModel(parent),
      _table(nullptr)
{

}

void TableColumnsModel::setTable(meow::db::TableEntity * table)
{
    removeData();
    _table = table;
    insertData();

    if (_table) {
        connect(_table->structure(),
                &meow::db::TableStructure::columnRelationChangedForIndex,
                [=](meow::db::TableColumn * column, meow::db::TableIndex * index){
                    Q_UNUSED(index);
                    refreshColumn(column);
                }
        );
    }
}

int TableColumnsModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return (int)Columns::Count;
}

int TableColumnsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (_table && _table->structure()) {
        return _table->structure()->columns().size();
    }
    return 0;
}

Qt::ItemFlags TableColumnsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    bool isEditable = isEditingAllowed(index.row(), index.column());

    // For bool columns we don't use standart editor,
    // so dont return Qt::ItemIsEditable;
    // Return Qt::ItemIsEnabled if bool column can be changed by the user

    if (isBoolColumn(index.column())) {
        if (isEditable) {
            flags |= Qt::ItemIsEnabled;
        }
    } else {
        flags |= Qt::ItemIsEnabled;
        if (isEditable) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

QVariant TableColumnsModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {

        case Columns::Name:
            return QString(tr("Name"));

        case Columns::DataType:
            return QString(tr("Datatype"));

        case Columns::Length:
            return QString(tr("Length/Set"));

        case Columns::Unsigned:
            return QString(tr("Unsigned"));

        case Columns::AllowNull:
            return QString(tr("Allow NULL"));

        case Columns::Zerofill:
            return QString(tr("Zerofill"));

        case Columns::Default:
            return QString(tr("Default"));

        case Columns::Comment:
            return QString(tr("Comment"));

        case Columns::Collation:
            return QString(tr("Collation"));

        case Columns::Expression:
            return QString(tr("Expression"));

        case Columns::Virtuality:
            return QString(tr("Virtuality"));

        default:
            break;
        }
    } else {
        return QString::number(section + 1);
    }

    return QVariant();
}

QVariant TableColumnsModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {

    case Qt::DisplayRole:
    case Qt::EditRole: // temp all the same
        return textDataAt(index.row(), index.column());

    case Qt::CheckStateRole:
        return checkStateAt(index.row(), index.column());

    case Qt::FontRole:
        return fontAt(index.row(), index.column());

    case Qt::ForegroundRole:
        return foregroundAt(index.row(), index.column());
    /*case Qt::TextAlignmentRole:
        if (isBoolColumn(index.column())) { // does not work
            return Qt::AlignCenter;
        }
        break;*/
    default:
        break;
    }

    return QVariant();
}

bool TableColumnsModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    if (editData(index, value)) {
        emit dataChanged(index, index);
        emit modified();
        return true;
    }

    return false;
}

int TableColumnsModel::insertEmptyDefaultRow(int afterIndex)
{
    int insertIndex = rowCount();
    if (afterIndex > -1 && afterIndex < rowCount()) {
        insertIndex = afterIndex + 1;
    }
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    int newRowIndex =  _table->structure()->insertEmptyDefaultColumn(afterIndex);
    endInsertRows();

    emit modified();

    return newRowIndex;
}

bool TableColumnsModel::removeRowAt(int index)
{
    if (!_table->structure()->canRemoveColumn(index)) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);
    _table->structure()->removeColumnAt(index);
    endRemoveRows();

    emit modified();

    return true;
}

bool TableColumnsModel::moveRowUp(int index)
{
    if (_table->structure()->canMoveColumnUp(index)) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
        _table->structure()->moveColumnUp(index);
        endMoveRows();

        emit modified();

        return true;
    }
    return false;
}

bool TableColumnsModel::moveRowDown(int index)
{
    if (_table->structure()->canMoveColumnDown(index)) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index + 2);
        _table->structure()->moveColumnDown(index);
        endMoveRows();
        emit modified();
        return true;
    }
    return false;
}

bool TableColumnsModel::canRemoveRow(int index) const
{
    // Not an error: a row in the table view is a column in the db table ;)
    return _table->structure()->canRemoveColumn(index);
}

bool TableColumnsModel::canMoveRowUp(int index) const
{
    return _table->structure()->canMoveColumnUp(index);
}

bool TableColumnsModel::canMoveRowDown(int index) const
{
    return _table->structure()->canMoveColumnDown(index);
}

void TableColumnsModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}
void TableColumnsModel::insertData()
{
    if (rowCount() > 0) {
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

void TableColumnsModel::refresh()
{
    removeData();
    insertData();
}

QVariant TableColumnsModel::textDataAt(int row, int col) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Name:
        return tableColumn->name();

    case Columns::DataType:
        return tableColumn->dataTypeName();

    case Columns::Length:
        return tableColumn->lengthSet();

    case Columns::Unsigned:
    case Columns::AllowNull:
    case Columns::Zerofill:
        return QVariant();

    case Columns::Default: {
        using colDef = meow::db::ColumnDefaultType;
        const QString onUpdCurTS = QString(" ON UPDATE CURRENT_TIMESTAMP");

        switch (tableColumn->defaultType()) {

        case colDef::None:
            return tr("No default");

        case colDef::Text:
            return tableColumn->defaultText();
        case colDef::TextUpdateTS:
            return tableColumn->defaultText() + onUpdCurTS;

        case colDef::Null:
            return QString("NULL");
        case colDef::NullUpdateTS:
            return QString("NULL") + onUpdCurTS;

        case colDef::CurTS:
            return QString("CURRENT_TIMESTAMP");
        case colDef::CurTSUpdateTS:
            return QString("CURRENT_TIMESTAMP") + onUpdCurTS;

        case colDef::AutoInc:
            return QString("AUTO_INCREMENT");

        default:
            break;
        }
        return QString();
    }

    case Columns::Comment:
        return tableColumn->comment();

    case Columns::Collation:
        return tableColumn->collation();

    case Columns::Expression:
        return QString("");

    case Columns::Virtuality:
        return QString("");

    default:
       return QVariant();
    }
}

QVariant TableColumnsModel::checkStateAt(int row, int col) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Unsigned:
        return tableColumn->isUnsigned() ? Qt::Checked : Qt::Unchecked;

    case Columns::AllowNull:
        return tableColumn->isAllowNull() ? Qt::Checked : Qt::Unchecked;

    case Columns::Zerofill:
        return tableColumn->isZeroFill() ? Qt::Checked : Qt::Unchecked;

    default:
        return QVariant();
    }
}

QVariant TableColumnsModel::fontAt(int row, int col) const
{
    Q_UNUSED(col);

    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
    bool isColumnInPK = _table->structure()->hasIndexForColumn(
        tableColumn->name(), meow::db::TableIndexClass::PrimaryKey);

    bool isBold = isColumnInPK;

    if (isBold) {
        QFont font;
        font.setBold(true);
        return font;
    }

    return QVariant();
}

QVariant TableColumnsModel::foregroundAt(int row, int col) const
{
    if (static_cast<Columns>(col) == Columns::DataType) {
        meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
        auto textSettings = meow::app()->settings()->textSettings();
        auto dataType = tableColumn->dataType();
        if (dataType != meow::db::DataTypeIndex::None) {
            return textSettings->colorForDataType(
                meow::db::categoryOfDataType(dataType)
            );
        }
    } else if (static_cast<Columns>(col) == Columns::Default) {
        meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
        auto textSettings = meow::app()->settings()->textSettings();
        return textSettings->colorForDefaultType(tableColumn->defaultType());
    }

    return QVariant();
}

inline bool TableColumnsModel::isBoolColumn(int col) const
{
    switch (static_cast<Columns>(col)) {

    case Columns::Unsigned:
    case Columns::AllowNull:
    case Columns::Zerofill:
        return true;
    default:
        return false;
    }
}

bool TableColumnsModel::isEditingAllowed(int row, int col) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Length:
        return meow::db::dataTypeHasLength(tableColumn->dataType());

    case Columns::Unsigned:
        return meow::db::dataTypeCanBeUnsigned(tableColumn->dataType());

    case Columns::AllowNull: {
        bool isColumnInPK = _table->structure()->hasIndexForColumn(
            tableColumn->name(), meow::db::TableIndexClass::PrimaryKey);
        return !isColumnInPK;
    }

    case Columns::Zerofill:
        return meow::db::dataTypeCanBeZeroFill(tableColumn->dataType());

    default:
        return true;
    }
}

int TableColumnsModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {

    case Columns::Name:
        return 150;
    case Columns::DataType:
        return 170;
    case Columns::Length:
        return 140;
    case Columns::Unsigned:
        return 100;
    case Columns::AllowNull:
        return 100;
    case Columns::Zerofill:
        return 100;
    case Columns::Default:
        return 220;
    case Columns::Comment:
        return 180;
    case Columns::Collation:
        return 150;
    case Columns::Expression:
        return 150;
    case Columns::Virtuality:
        return 150;

    default:
        break;
    }
    return 0;
}

meow::db::ColumnDefaultType TableColumnsModel::defaultType(int row) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
    return tableColumn->defaultType();
}

const QString TableColumnsModel::defaultText(int row) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    using colDef = meow::db::ColumnDefaultType;
    if (tableColumn->defaultType() == colDef::Text ||
        tableColumn->defaultType() == colDef::TextUpdateTS) {

        return tableColumn->defaultText();
    }

    return QString();
}

bool TableColumnsModel::isDefaultAutoIncEnabled(int row) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
    return meow::db::categoryOfDataType(tableColumn->dataType())
            == meow::db::DataTypeCategoryIndex::Integer;
}

bool TableColumnsModel::isDefaultCurrentTimeStampEnabled(int row) const
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    if (tableColumn->dataType() == meow::db::DataTypeIndex::Timestamp) {
        return true;
    } else if (tableColumn->dataType() == meow::db::DataTypeIndex::DateTime) {
        return _table->connection()->serverVersionInt() >= 50605;
    }
    return false;
}

bool TableColumnsModel::isDefaultOnUpdCurTsEnabled(int row) const
{
    return isDefaultCurrentTimeStampEnabled(row);
}

bool TableColumnsModel::setDefaultValue(int row,
                     meow::db::ColumnDefaultType type,
                     const QString & text)
{
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);
    tableColumn->setDefaultType(type);
    tableColumn->setDefaultText(text);

    if (type == meow::db::ColumnDefaultType::Null ||
        type == meow::db::ColumnDefaultType::NullUpdateTS) {
        tableColumn->setAllowNull(true);
    }

    QModelIndex index = createIndex(row, (int)Columns::Default);

    emit dataChanged(index, index);
    emit modified();

    return true;
}

const QStringList TableColumnsModel::collationList() const
{
    return _table->connection()->collationList();
}

bool TableColumnsModel::setColumnDataType(const QModelIndex &index,
                                          const QVariant &value)
{
    int row = index.row();
    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    if (!value.canConvert<int>()) {
        return false;
    }

    auto dataType = static_cast<meow::db::DataTypeIndex>(value.toInt());
    tableColumn->setDataType(dataType);

    if (!meow::db::dataTypeHasLength(dataType)) {
        tableColumn->setLengthSet(QString());
    }
    // TODO: default length/set

    using ColDef = meow::db::ColumnDefaultType;

    auto typeCategory = meow::db::categoryOfDataType(dataType);

    switch (typeCategory) {
    case meow::db::DataTypeCategoryIndex::Integer:
    case meow::db::DataTypeCategoryIndex::Float:

        if (tableColumn->defaultType() == ColDef::CurTS ||
            tableColumn->defaultType() == ColDef::CurTSUpdateTS) {
            tableColumn->setDefaultType(ColDef::None);
        } else if (tableColumn->defaultType() == ColDef::TextUpdateTS) {
            tableColumn->setDefaultType(ColDef::Text);
        } else if (tableColumn->defaultType() == ColDef::NullUpdateTS) {
            tableColumn->setDefaultType(ColDef::Null);
        }
        break;

    case meow::db::DataTypeCategoryIndex::Text:
    case meow::db::DataTypeCategoryIndex::Binary:
    case meow::db::DataTypeCategoryIndex::Spatial:
    case meow::db::DataTypeCategoryIndex::Other:
        if (tableColumn->defaultType() == ColDef::CurTS ||
            tableColumn->defaultType() == ColDef::CurTSUpdateTS ||
            tableColumn->defaultType() == ColDef::AutoInc) {
            tableColumn->setDefaultType(ColDef::None);
        } else if (tableColumn->defaultType() == ColDef::NullUpdateTS) {
            tableColumn->setDefaultType(ColDef::Null);
        }
        break;

    case meow::db::DataTypeCategoryIndex::Temporal:
        if (tableColumn->defaultType() == ColDef::AutoInc) {
            tableColumn->setDefaultType(ColDef::None);
        }
        break;

    default:
        break;
    }

    return true;
}

bool TableColumnsModel::editData(const QModelIndex &index, const QVariant &value)
{
    int row = index.row();
    int col = index.column();

    meow::db::TableColumn * tableColumn = _table->structure()->columns().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Name: {
        tableColumn->setName(value.toString());
        return true;
    }

    case Columns::DataType: {
        return setColumnDataType(index, value);
    }

    case Columns::Length: {
        tableColumn->setLengthSet(value.toString());
        return true;
    }

    case Columns::Comment: {
        tableColumn->setComment(value.toString());
        return true;
    }

    case Columns::Collation: {
        tableColumn->setCollation(value.toString());
        return true;
    }

    case Columns::Unsigned: {
        if (value.canConvert<int>()) {
            bool checked = static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked;
            tableColumn->setIsUnsigned(checked);
            return true;
        }
        return false;
    }

    case Columns::AllowNull: {
        if (value.canConvert<int>()) {
            bool checked = static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked;
            tableColumn->setAllowNull(checked);
            return true;
        }
        return false;
    }

    case Columns::Zerofill: {

        if (value.canConvert<int>()) {
            bool checked = static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked;

            tableColumn->setIsZeroFill(checked);

            return true;
        }
        return false;
    }

    default:
        break;
    }

    return false;
}

void TableColumnsModel::refreshColumn(meow::db::TableColumn * column)
{
    int rowIndex = _table->structure()->columns().indexOf(column);
    if (rowIndex >= 0) {
        emit dataChanged(
                    index(rowIndex, 0),
                    index(rowIndex, columnCount() - 1)
        );
    }
}

} // namespace db
} // namespace models
} // namespace meow
