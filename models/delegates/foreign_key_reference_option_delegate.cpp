#include "foreign_key_reference_option_delegate.h"
#include <QComboBox>
#include "models/forms/table_foreign_keys_model.h"

namespace meow {
namespace models {
namespace delegates {

ForeignKeyReferenceOptionDelegate::ForeignKeyReferenceOptionDelegate(
        QObject * parent
    )
    :QStyledItemDelegate(parent)
{

}

QWidget * ForeignKeyReferenceOptionDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new QComboBox(parent);
}

void ForeignKeyReferenceOptionDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    auto model = static_cast<const models::forms::TableForeignKeysModel *>(
        index.model());

    comboBox->addItems(model->referenceOptions());

    QString value = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void ForeignKeyReferenceOptionDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void ForeignKeyReferenceOptionDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

} // namespace delegates
} // namespace models
} // namespace meow
