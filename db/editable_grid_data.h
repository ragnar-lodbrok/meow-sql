#ifndef DB_EDITABLE_GRID_DATA_H
#define DB_EDITABLE_GRID_DATA_H

#include <QStringList>
#include <QVariant>

namespace meow {
namespace db {

using GridDataRow = QStringList; // TODO QVector<QString> is faster ?

// Intent: data container for editing in grid/table form
class EditableGridData
{
public:
    EditableGridData();

    void clear() { _rows.clear(); }
    void reserve(int alloc);
    void reserveForAppend(int append);

    inline void appendRow(const GridDataRow & row) {
        _rows.append(row);
    }

    inline int rowsCount() const {
        return _rows.size();
    }

    const QString & dataAt(std::size_t row, std::size_t col) const {
        return _rows.at(row).at(col);
    }

    bool setData(int row, int col, const QVariant &value) {
        _rows[row][col] = value.toString();
        return true;
    }

private:
    QList<GridDataRow> _rows;
};

} // namespace db
} // namespace meow

#endif // DB_EDITABLE_GRID_DATA_H
