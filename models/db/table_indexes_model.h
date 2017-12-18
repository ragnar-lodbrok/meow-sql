#ifndef MODELS_DB_TABLE_INDEXES_MODEL_H
#define MODELS_DB_TABLE_INDEXES_MODEL_H

#include <QAbstractItemModel>

namespace meow {

namespace db {
class TableEntity;
}

namespace models {
namespace db {

class TableIndexesModel : public QAbstractItemModel
{
public:

    enum class Columns {
        Name = 0,
        Type,
        Algorithm,
        Count
    };

    TableIndexesModel(QObject * parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

    int columnWidth(int column) const;

    //bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

private:

};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_TABLE_INDEXES_MODEL_H
