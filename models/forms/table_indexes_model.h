#ifndef MODELS_FORMS_TABLE_INDEXES_MODEL_H
#define MODELS_FORMS_TABLE_INDEXES_MODEL_H

#include <QAbstractItemModel>
#include "table_indexes_model_item.h"

namespace meow {

namespace db {
class TableEntity;
}

namespace models {
namespace forms {


class TableIndexesModel : public QAbstractItemModel,
                          public ITableIndexesModelItem
{
    Q_OBJECT

public:

    enum class Columns {
        Name = 0,
        Type,
        Algorithm,
        Count
    };

    TableIndexesModel(QObject * parent = nullptr);
    virtual ~TableIndexesModel();

    void setTable(meow::db::TableEntity * table);
    bool hasTable() const { return _table != nullptr; }

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(
            int row, int column,
            const QModelIndex &parentIndex = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

    int columnWidth(int column) const;

    //bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    virtual int childCount() const override { return _items.size(); }
    virtual Type type() const override { return Type::Root; }

    virtual ITableIndexesModelItem * child(int row) const override;
    virtual int rowOf(ITableIndexesModelItem * child) const override;

    bool canAddIndex() const;
    int insertEmptyDefaultIndex();

    bool canAddColumn(const QModelIndex & curIndex) const;
    bool canRemove(const QModelIndex & curIndex) const;
    bool canRemoveAll() const;
    bool canMoveUp(const QModelIndex & curIndex) const;
    bool canMoveDown(const QModelIndex & curIndex) const;

    Q_SIGNAL void modified();

private:

    void reinitItems();
    ITableIndexesModelItem * rootItem() const;

    void removeData();
    void insertData();

    meow::db::TableEntity * _table;
    QList<ITableIndexesModelItem *> _items;

};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TABLE_INDEXES_MODEL_H
