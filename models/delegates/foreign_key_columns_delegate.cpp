#include "foreign_key_columns_delegate.h"
#include <QComboBox>
#include "models/forms/table_foreign_keys_model.h"

namespace meow {
namespace models {
namespace delegates {

ForeignKeyColumnsDelegate::ForeignKeyColumnsDelegate(
        QObject * parent
    )
    :QStyledItemDelegate(parent)
{

}

QWidget * ForeignKeyColumnsDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    // TODO: multi select
    return new QComboBox(parent);
}

void ForeignKeyColumnsDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    auto model = static_cast<const models::forms::TableForeignKeysModel *>(
        index.model());

    comboBox->addItems( model->allColumns() );

    QString valuesString = index.model()->data(index, Qt::EditRole).toString();
    QStringList values = valuesString.split(',');
    // TODO: should be multiple selection
    if (!values.isEmpty()) {
        comboBox->setCurrentIndex(comboBox->findText(values[0]));
    }
}

void ForeignKeyColumnsDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void ForeignKeyColumnsDelegate::updateEditorGeometry(
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
