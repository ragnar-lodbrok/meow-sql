#include "edit_query_data_delegate.h"
#include <QLineEdit>
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
    // TODO: custom editors for non-string types
    //return new QLineEdit(parent);
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

/*void EditQueryDataDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
    //QString value = index.model()->data(index, Qt::EditRole).toString();
    //auto lineEdit = static_cast<QLineEdit *>(editor);
    //lineEdit->setText(value);
}*/

/*void EditQueryDataDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
    //auto lineEdit = static_cast<QLineEdit *>(editor);
    //QVariant curData = lineEdit->text();
    //model->setData(index, curData, Qt::EditRole);
}*/

} // namespace delegates
} // namespace models
} // namespace meow
