#include "table_column_collation_delegate.h"
#include <QComboBox>
#include "models/db/table_columns_model.h"

namespace meow {
namespace models {
namespace delegates {

TableColumnCollationDelegate::TableColumnCollationDelegate(QObject * parent)
    :QStyledItemDelegate(parent)
{

}

QWidget * TableColumnCollationDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new QComboBox(parent);
}

void TableColumnCollationDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    auto model = static_cast<const models::db::TableColumnsModel *>(
        index.model());

    comboBox->addItem("");
    comboBox->addItems(model->collationList());

    QString value = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void TableColumnCollationDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<QComboBox *>(editor);
    QVariant curData = comboBox->currentText();
    model->setData(index, curData, Qt::EditRole);
}

void TableColumnCollationDelegate::updateEditorGeometry(
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
