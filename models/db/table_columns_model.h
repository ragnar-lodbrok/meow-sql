#ifndef TABLE_COLUMNS_MODEL_H
#define TABLE_COLUMNS_MODEL_H

#include <QAbstractTableModel>

namespace meow {

namespace db {
    class TableEntity;
    class TableColumn;
    enum class ColumnDefaultType;
}

namespace models {
namespace db {

class TableColumnsModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum class Columns {
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

    void setTable(meow::db::TableEntity * table);
    bool hasTable() const { return _table != nullptr; }
    void refresh();

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;

    int insertEmptyDefaultRow(int afterIndex = -1);

    bool canRemoveRow(int index) const;
    bool canMoveRowUp(int index) const;
    bool canMoveRowDown(int index) const;

    bool removeRowAt(int index);
    bool moveRowUp(int index);
    bool moveRowDown(int index);

    meow::db::ColumnDefaultType defaultType(int row) const;
    const QString defaultText(int row) const;

    bool isDefaultAutoIncEnabled(int row) const;
    bool isDefaultCurrentTimeStampEnabled(int row) const;
    bool isDefaultOnUpdCurTsEnabled(int row) const;

    bool setDefaultValue(int row,
                         meow::db::ColumnDefaultType type,
                         const QString & text);

    const QStringList collationList() const;

    Q_SIGNAL void modified();

private:

    bool editData(const QModelIndex &index, const QVariant &value);

    QVariant textDataAt(int row, int col) const;
    QVariant checkStateAt(int row, int col) const;
    QVariant fontAt(int row, int col) const;
    QVariant foregroundAt(int row, int col) const;

    bool isBoolColumn(int col) const;
    bool isEditingAllowed(int row, int col) const;

    void removeData();
    void insertData();

    bool setColumnDataType(const QModelIndex &index, const QVariant &value);

    void refreshColumn(meow::db::TableColumn * column);

    meow::db::TableEntity * _table;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // TABLE_COLUMNS_MODEL_H
