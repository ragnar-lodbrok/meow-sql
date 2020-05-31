#ifndef UI_TABLE_CELL_LINE_EDIT_H
#define UI_TABLE_CELL_LINE_EDIT_H

#include <QtWidgets>

namespace meow {
namespace ui {

class TableCellLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TableCellLineEdit(QWidget * parent = nullptr);

    QLineEdit * lineEdit() const {
        return _lineEdit;
    }

    Q_SLOT void openPopupEditor();

    Q_SIGNAL void popupEditorClosed(bool accepted);

    // TODO make wider if text doesn't fit, see QExpandingLineEdit

private:



    QLineEdit * _lineEdit;
    QPushButton * _openPopupEditorButton;
};

} // namespace ui
} // namespace meow

#endif // UI_TABLE_CELL_LINE_EDIT_H
