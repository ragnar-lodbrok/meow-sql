#include "table_foreign_keys_model.h"
#include "ui/presenters/table_info_form.h"
#include "db/entity/table_entity.h"
#include "db/entity/entity_filter.h"
#include "db/connection.h"
#include <QIcon>

namespace meow {
namespace ui {
namespace models {

TableForeignKeysModel::TableForeignKeysModel(presenters::TableInfoForm * parent)
    : QAbstractTableModel(parent),
      _tableForm(parent),
      _table(nullptr)
{

}

void TableForeignKeysModel::setTable(meow::db::TableEntity * table)
{
    removeData();
    _table = table;
    insertData();
}

int TableForeignKeysModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int TableForeignKeysModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (_table && _table->structure()) {
        return _table->structure()->foreignKeys().size();
    }
    return 0;
}

Qt::ItemFlags TableForeignKeysModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return {};
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (isEditingSupported() && isEditingAllowed(index.row(), index.column())) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

QVariant TableForeignKeysModel::headerData(
                                    int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {

        case Columns::Name:
            return QString(tr("Key name"));

        case Columns::Columns:
            return QString(tr("Columns"));

        case Columns::ReferenceTable:
            return QString(tr("Reference table"));

        case Columns::ForeignColumns:
            return QString(tr("Foreign columns"));

        case Columns::OnUpdate:
            return QString(tr("On UPDATE"));

        case Columns::OnDelete:
            return QString(tr("On DELETE"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant TableForeignKeysModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {

    case Qt::DisplayRole:
        return textDataAt(index.row(), index.column());

    case Qt::EditRole:
        return editDataAt(index.row(), index.column());

    case Qt::DecorationRole:
        if (static_cast<Columns>(index.column()) == Columns::Name) {
            // TODO: move to Settings::icons
            static const QIcon icon = QIcon(":/icons/table_relationship.png");
            return icon;
        }

    default:
        break;
    }

    return QVariant();
}

bool TableForeignKeysModel::setData(const QModelIndex &index,
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

int TableForeignKeysModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {

    case Columns::Name:
        return 220;

    case Columns::ReferenceTable:
        return 170;

    default:
       return 130;
    }
}

bool TableForeignKeysModel::canAddKey() const
{
    if (!isEditingSupported()) return false;
    return _tableForm->supportsForeignKeys();
}

int TableForeignKeysModel::insertEmptyDefaultKey()
{
    int insertIndex = rowCount();
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    int newRowIndex =  _table->structure()->insertEmptyDefaultForeignKey();
    endInsertRows();

    emit modified();

    return newRowIndex;
}

bool TableForeignKeysModel::canRemoveKey(const QModelIndex & curIndex) const
{
    if (_table == nullptr || curIndex.isValid() == false) return false;

    if (!isEditingSupported()) return false;

    return _table->structure()->canRemoveKey(curIndex.row());
}

bool TableForeignKeysModel::removeKey(const QModelIndex & curIndex)
{
    Q_UNUSED(curIndex);
    if (!canRemoveKey(curIndex)) {
        return false;
    }
    int row = curIndex.row();

    beginRemoveRows(QModelIndex(), row, row);
    _table->structure()->removeKeyAt(row);
    endRemoveRows();

    emit modified();

    return true;
}

bool TableForeignKeysModel::canRemoveAllKeys() const
{
    if (!isEditingSupported()) return false;
    return _table->structure()->canRemoveAllKeys();
}

void TableForeignKeysModel::removeAllKeys()
{
    if (!canRemoveAllKeys()) return;

    removeData();
    _table->structure()->removeAllKeys();

    emit modified();
}

QStringList TableForeignKeysModel::referenceOptions() const
{
    return meow::db::referenceOptionNames();
}

QStringList TableForeignKeysModel::referenceTables() const
{
    std::unique_ptr<db::EntityFilter> filter
        = _table->connection()->entityFilter();
    QList<db::TableEntity *> tables = filter->tablesWithForeignKeySupport(
        meow::db::databaseName(_table)
    );
    QStringList tableNames;
    for (db::TableEntity * table : tables) {
        tableNames << table->name();
    }
    return tableNames;
}

QStringList TableForeignKeysModel::allColumns() const
{
    return db::tableColumnNames(_table->structure());
}

QStringList TableForeignKeysModel::referenceColumns(int row) const
{
    meow::db::ForeignKey * key = _table->structure()->foreignKeys().at(row);

    db::TableEntity * referenceTable = key->referenceTable();
    if (!referenceTable) {
        return {};
    }
    return db::tableColumnNames(referenceTable->structure());
}

void TableForeignKeysModel::removeAllForeignKeyColumnsByName(
    const QString & columnName)
{
    auto fKeys = _table->structure()->foreignKeys();
    for (auto & fKey : fKeys) {
        fKey->removeColumnByName(columnName);
    }
}

bool TableForeignKeysModel::isEditingSupported() const
{
    if (_table == nullptr) return false;
    return _table->connection()->features()->supportsEditingTablesStructure();
}

bool TableForeignKeysModel::editData(const QModelIndex &index,
                                     const QVariant &value)
{
    int row = index.row();
    int col = index.column();

    meow::db::ForeignKey * key = _table->structure()->foreignKeys().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Name: {
        key->setName(value.toString());
        key->setIsCustomName(true);
        return true;
    }

    case Columns::ReferenceTable: {
        key->setReferenceTableName(value.toString());
        if (!key->isCustomName()) {
            key->setName(genName(key));
        }
        return true;
    }

    case Columns::Columns: {
        QStringList columnNames = value.toStringList();
        key->setColumns(columnNames);
        return true;
    }

    case Columns::ForeignColumns: {
        key->referenceColumns() = value.toStringList();
        return true;
    }

    case Columns::OnUpdate: {
        key->setOnUpdate(value.toString());
        return true;
    }

    case Columns::OnDelete: {
        key->setOnDelete(value.toString());
        return true;
    }

    default:
        break;
    }

    return false;
}

bool TableForeignKeysModel::isEditingAllowed(int row, int col) const
{
    Q_UNUSED(row);
    Q_UNUSED(col);

    // TODO
    return true;
}

QString TableForeignKeysModel::textDataAt(int row, int col) const
{
    meow::db::ForeignKey * fKey = _table->structure()->foreignKeys().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Name:
        return fKey->name();

    case Columns::Columns:
        return fKey->columnNames().join(',');

    case Columns::ReferenceTable:
        return fKey->referenceTableName();

    case Columns::ForeignColumns:
        return fKey->referenceColumns().join(',');

    case Columns::OnUpdate:
        return referenceOptionToStr(fKey->onUpdate());

    case Columns::OnDelete:
        return referenceOptionToStr(fKey->onDelete());

    default:
       return QString();
    }
}

QVariant TableForeignKeysModel::editDataAt(int row, int col) const
{
    meow::db::ForeignKey * fKey = _table->structure()->foreignKeys().at(row);

    switch (static_cast<Columns>(col)) {
    case Columns::Columns:
        return fKey->columnNames();
    case Columns::ForeignColumns:
        return fKey->referenceColumns();
    default:
        return textDataAt(row, col);
    }
}

void TableForeignKeysModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}

void TableForeignKeysModel::insertData()
{
    if (rowCount() > 0) {
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

QString TableForeignKeysModel::genName(db::ForeignKey * key) const
{
    QString name = "fk_" + _table->name() + "_" + key->referenceTableName();

    QList<db::ForeignKey *> & keys = _table->structure()->foreignKeys();

    auto nameIsUsed = [&](const QString & nameToCheck)->bool {
        for (db::ForeignKey * keyIt : keys) {
            if (nameToCheck == keyIt->name()) {
                return true;
            }
        }
        return false;
    };

    int i = 0;
    while (nameIsUsed(name)) {
        name = "fk_" + _table->name() + "_" + key->referenceTableName();
        name += "_" + QString::number(++i);
    }

    return name;
}

} // namespace models
} // namespace ui
} // namespace meow
