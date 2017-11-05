#ifndef TABLE_COLUMNS_MODEL_H
#define TABLE_COLUMNS_MODEL_H

#include <QAbstractTableModel>

namespace meow {

namespace db {
    class TableEntity;
}

namespace models {
namespace db {

class TableColumnsModel : public QAbstractTableModel
{
public:

    enum class Columns {
        Number = 0,
        Name,
        DataType,
        Length,
        Unsigned,
        AllowNull,
        Zerofill,
        Default,
        Comment,
        Collation,
        Expression,
        Virtuality,

        Count
    };

    TableColumnsModel(QObject * parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    meow::db::TableEntity * _table;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // TABLE_COLUMNS_MODEL_H
