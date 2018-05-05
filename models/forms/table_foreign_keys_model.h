#ifndef MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H
#define MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H

#include <QAbstractItemModel>

namespace meow {

namespace db {
class TableEntity;
}

namespace models {
namespace forms {

class TableInfoForm;

class TableForeignKeysModel : public QAbstractTableModel
{
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

    TableForeignKeysModel(TableInfoForm * parent);

    void setTable(meow::db::TableEntity * table);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
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

private:

    QString textDataAt(int row, int col) const;

    void removeData();
    void insertData();

    TableInfoForm * _tableForm;
    meow::db::TableEntity * _table;

};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TABLE_FOREIGN_KEYS_MODEL_H
