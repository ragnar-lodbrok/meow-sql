#include "edit_query_data_delegate.h"
#include "ui/common/table_cell_line_edit.h"
#include <QDebug>

namespace meow {
namespace models {
namespace delegates {

EditQueryDataDelegate::EditQueryDataDelegate(QObject * parent)
    :QStyledItemDelegate(parent),
     _editor(nullptr)
{

}

QWidget * EditQueryDataDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    _editor = QStyledItemDelegate::createEditor(parent, option, index);
    return _editor;

}

void EditQueryDataDelegate::destroyEditor(QWidget *editor,
                   const QModelIndex &index) const
{
    _editor = nullptr;
    QStyledItemDelegate::destroyEditor(editor, index);
}

void EditQueryDataDelegate::commit()
{
    if (_editor) {
        emit commitData(_editor);
        emit closeEditor(_editor);
        _editor = nullptr;
    }
}

void EditQueryDataDelegate::discard()
{
    if (_editor) {
        emit closeEditor(_editor);
        _editor = nullptr;
    }
}

// -----------------------------------------------------------------------------

EditTextQueryDataDelegate::EditTextQueryDataDelegate(QObject * parent)
    :EditQueryDataDelegate(parent)
{

}

QWidget * EditTextQueryDataDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    _editor = new ui::TableCellLineEdit(parent);

    return _editor;
}

void EditTextQueryDataDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto lineEdit = static_cast<ui::TableCellLineEdit *>(editor)->lineEdit();
    lineEdit->setText(value);
    lineEdit->setCursorPosition(value.length());
    lineEdit->selectAll();

    QTimer::singleShot(0, lineEdit, SLOT(setFocus())); // trick
}

void EditTextQueryDataDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto lineEdit = static_cast<ui::TableCellLineEdit *>(editor)->lineEdit();
    QVariant curData = lineEdit->text();
    model->setData(index, curData, Qt::EditRole);
}

QString EditTextQueryDataDelegate::displayText(const QVariant &value,
                                               const QLocale &locale) const
{
    // TODO: better paint \r and \n so you can distinguish \\n from \n ?

    if (value.canConvert<QString>()) {

        return value.toString()
                .replace(QChar('\n'), QChar(' '), Qt::CaseInsensitive)
                .replace(QChar('\r'), QChar(' '), Qt::CaseInsensitive);

    } else {
        return QStyledItemDelegate::displayText(value, locale);
    }
}

} // namespace delegates
} // namespace models
} // namespace meow
