#ifndef MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H
#define MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H

#include <QAbstractItemModel>

namespace meow {

namespace db {
class TableEntity;
class ForeignKey;
}

namespace models {
namespace forms { // TODO move to db namespace

class TableInfoForm;

class TableForeignKeysModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum class Columns {
        Name = 0,
        Columns,
        ReferenceTable,
        ForeignColumns,
        OnUpdate,
        OnDelete,
        Count
    };

    explicit TableForeignKeysModel(TableInfoForm * parent);

    void setTable(meow::db::TableEntity * table);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;

    bool canAddKey() const;
    int insertEmptyDefaultKey();

    bool canRemoveKey(const QModelIndex & curIndex) const;
    bool removeKey(const QModelIndex & curIndex);

    bool canRemoveAllKeys() const;
    void removeAllKeys();

    QStringList referenceOptions() const;
    QStringList referenceTables() const;
    QStringList allColumns() const;
    QStringList referenceColumns(int row) const;

    void removeAllForeignKeyColumnsByName(const QString & columnName);

    Q_SIGNAL void modified();

private:

    bool isEditingSupported() const;

    bool editData(const QModelIndex &index, const QVariant &value);
    bool isEditingAllowed(int row, int col) const;

    QString textDataAt(int row, int col) const;
    QVariant editDataAt(int row, int col) const;

    void removeData();
    void insertData();

    QString genName(meow::db::ForeignKey * key) const;

    TableInfoForm * _tableForm;
    meow::db::TableEntity * _table;

};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H
