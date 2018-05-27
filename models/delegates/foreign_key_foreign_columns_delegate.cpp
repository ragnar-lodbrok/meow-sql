#include "foreign_key_foreign_columns_delegate.h"
#include <QComboBox>
#include "models/forms/table_foreign_keys_model.h"

namespace meow {
namespace models {
namespace delegates {

ForeignKeyForeignColumnsDelegate::ForeignKeyForeignColumnsDelegate(
        QObject * parent
    )
    :QStyledItemDelegate(parent)
{

}

QWidget * ForeignKeyForeignColumnsDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    // TODO: multi select
    return new QComboBox(parent);
}

void ForeignKeyForeignColumnsDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    auto model = static_cast<const models::forms::TableForeignKeysModel *>(
        index.model());

    comboBox->addItems(model->referenceColumns(index.row()));

    QString valuesString = index.model()->data(index, Qt::EditRole).toString();
    QStringList values = valuesString.split(',');
    // TODO: should be multiple selection
    comboBox->setCurrentIndex(comboBox->findText(values[0]));
}

void ForeignKeyForeignColumnsDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void ForeignKeyForeignColumnsDelegate::updateEditorGeometry(
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
