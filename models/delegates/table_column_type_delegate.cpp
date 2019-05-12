#include "table_column_type_delegate.h"
#include "ui/common/data_type_combo_box.h"

namespace meow {
namespace models {
namespace delegates {

TableColumnTypeDelegate::TableColumnTypeDelegate(db::TableColumnsModel * model)
    :QStyledItemDelegate(model),
     _model(model)
{

}

QWidget * TableColumnTypeDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new ui::DataTypeComboBox(parent);
}

void TableColumnTypeDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto comboBox = static_cast<ui::DataTypeComboBox *>(editor);
    comboBox->fillData(_model->dataTypeNames());
    QString value = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentIndex(comboBox->findText(value));
}

void TableColumnTypeDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto comboBox = static_cast<ui::DataTypeComboBox *>(editor);
    QVariant curData = comboBox->currentData();
    model->setData(index, curData, Qt::EditRole);
}

void TableColumnTypeDelegate::updateEditorGeometry(
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
