#ifndef UI_TABLE_CELL_LINE_EDIT_H
#define UI_TABLE_CELL_LINE_EDIT_H


#include <QtWidgets>

namespace meow {
namespace ui {

class TableCellLineEdit : public QWidget
{
public:
    explicit TableCellLineEdit(QWidget * parent = nullptr);

    QLineEdit * lineEdit() const {
        return _lineEdit;
    }

    // TODO make wider if text doesn't fit, see QExpandingLineEdit

private:


    QLineEdit * _lineEdit;
    QPushButton * _pushButton;
};

} // namespace ui
} // namespace meow

#endif // UI_TABLE_CELL_LINE_EDIT_H
