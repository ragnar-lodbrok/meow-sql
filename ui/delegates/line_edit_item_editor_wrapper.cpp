#include "line_edit_item_editor_wrapper.h"
#include "ui/common/table_cell_line_edit.h"
#include "helpers/text.h"

namespace meow {
namespace ui {
namespace delegates {

QWidget * LineEditItemEditorWrapper::createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    _editor = new ui::TableCellLineEdit(parent);

    connect(static_cast<ui::TableCellLineEdit *>(_editor),
            &ui::TableCellLineEdit::popupEditorClosed,
            this,
            &LineEditItemEditorWrapper::onPopupTextEditorClosed,
            Qt::QueuedConnection);

    return _editor;
}

void LineEditItemEditorWrapper::onPopupTextEditorClosed(bool accepted)
{
    if (accepted) {
        _delegate->commit();
    } else {
        _delegate->discard();
    }
}

void LineEditItemEditorWrapper::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto cellLineEdit = static_cast<ui::TableCellLineEdit *>(editor);
    auto lineEdit = cellLineEdit->lineEdit();
    lineEdit->setText(value);

    if (value.length() > 10*1024 || helpers::hasLineBreaks(value)) {
        cellLineEdit->openPopupEditor();
    } else {
        lineEdit->setCursorPosition(value.length());
        lineEdit->selectAll();

        QTimer::singleShot(0, lineEdit, SLOT(setFocus())); // trick
    }
}

void LineEditItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    auto lineEdit = static_cast<ui::TableCellLineEdit *>(editor)->lineEdit();
    QVariant curData = lineEdit->text();
    model->setData(index, curData, Qt::EditRole);
}


} // namespace delegates
} // namespace ui
} // namespace meow
