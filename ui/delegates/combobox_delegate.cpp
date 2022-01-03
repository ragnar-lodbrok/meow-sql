#include "combobox_delegate.h"
#include <QComboBox>

namespace meow {
namespace ui {
namespace delegates {

ComboboxDelegate::ComboboxDelegate(QObject * parent)
    : QStyledItemDelegate(parent)
    , _listDataRole(Qt::EditRole)
    , _currentValueDataRole(Qt::DisplayRole)
    , _isComboboxEditable(false)
{

}

QWidget * ComboboxDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox * combobox = new QComboBox(parent);

    combobox->setEditable(_isComboboxEditable);

    return combobox;
}

void ComboboxDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QStringList values =
            index.model()->data(index, _listDataRole).toStringList();
    comboBox->addItems(values);

    QString value =
            index.model()->data(index, _currentValueDataRole).toString();
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void ComboboxDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void ComboboxDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

} // namespace delegates
} // namespace ui
} // namespace meow
