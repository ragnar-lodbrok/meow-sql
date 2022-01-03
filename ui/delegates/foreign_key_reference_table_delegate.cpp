#include "foreign_key_reference_table_delegate.h"
#include <QComboBox>
#include "ui/models/table_foreign_keys_model.h"

namespace meow {
namespace ui {
namespace delegates {

ForeignKeyReferenceTableDelegate::ForeignKeyReferenceTableDelegate(
        QObject * parent
    )
    :QStyledItemDelegate(parent)
{

}

QWidget * ForeignKeyReferenceTableDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new QComboBox(parent);
}

void ForeignKeyReferenceTableDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    auto model = static_cast<const models::TableForeignKeysModel *>(
        index.model());

    comboBox->addItems(model->referenceTables());

    QString value = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void ForeignKeyReferenceTableDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void ForeignKeyReferenceTableDelegate::updateEditorGeometry(
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
