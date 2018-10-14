#ifndef DB_EDITABLE_GRID_DATA_H
#define DB_EDITABLE_GRID_DATA_H

#include <QStringList>
#include <QVariant>
#include <memory>
#include <QDebug>

namespace meow {
namespace db {

using GridDataRow = QStringList; // TODO QVector<QString> is faster ?

struct EditableGridDataRow
{
public:
    EditableGridDataRow(const GridDataRow & aData, std::size_t aRowNumber)
        : data(aData), // copy data to edit
          rowNumber(aRowNumber)
    {

    }
    GridDataRow data;
    std::size_t rowNumber;
};

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

        if (_editableRow && _editableRow->rowNumber == row) {
            return _editableRow->data.at(col);
        }

        return _rows.at(row).at(col);
    }

    const QString & notModifiedDataAt(std::size_t row, std::size_t col) const {
        return _rows.at(row).at(col);
    }

    bool setData(int row, int col, const QVariant &value) {
        if (dataAt(row, col) == value.toString()) {
            return false;
        }
        createUpdateRow(row);
        _editableRow->data[col] = value.toString();
        return true;
    }

    bool isModified() const {
        return _editableRow != nullptr;
    }

    int discardModifications() {
        int editableRowNumber = -1;
        if (_editableRow) {
            editableRowNumber = _editableRow->rowNumber;
        }
        _editableRow.reset();
        return editableRowNumber;
    }

    int applyModifications() {
        if (!isModified()) return -1;

        int editableRowNumber = _editableRow->rowNumber;
        _rows[_editableRow->rowNumber] = _editableRow->data;
        _editableRow.reset();

        return editableRowNumber;
    }

    EditableGridDataRow * editableRow() const {
        return _editableRow.get();
    }

private:

    void createUpdateRow(std::size_t rowNumber) {
        if (!_editableRow || _editableRow->rowNumber != rowNumber) {
            _editableRow = std::make_shared<EditableGridDataRow>(
                _rows[rowNumber],
                rowNumber
            );
        }
    }

    QList<GridDataRow> _rows;
    std::shared_ptr<EditableGridDataRow> _editableRow;
};

} // namespace db
} // namespace meow

#endif // DB_EDITABLE_GRID_DATA_H
