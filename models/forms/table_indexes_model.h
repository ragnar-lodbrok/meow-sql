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
public:

    enum class Columns {
        Name = 0,
        Type,
        Algorithm,
        Count
    };

    TableIndexesModel(QObject * parent = nullptr);
    ~TableIndexesModel();

    void setTable(meow::db::TableEntity * table);

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
