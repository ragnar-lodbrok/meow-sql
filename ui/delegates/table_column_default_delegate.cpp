#include "table_column_default_delegate.h"
#include "ui/common/table_column_default_editor.h"
#include "ui/models/table_columns_model.h"
#include "ui/common/geometry_helpers.h"

namespace meow {
namespace ui {
namespace delegates {

TableColumnDefaultDelegate::TableColumnDefaultDelegate(QObject * parent)
    :QStyledItemDelegate(parent)
{

}

QWidget * TableColumnDefaultDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new ui::TableColumnDefaultEditor(parent);
}

void TableColumnDefaultDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{

    auto defaultEditor = static_cast<ui::TableColumnDefaultEditor *>(editor);
    auto model = static_cast<const models::TableColumnsModel *>(index.model());

    defaultEditor->setAutoIncEditable(
        model->isDefaultAutoIncEnabled(index.row()));
    defaultEditor->setCurrentTimeStampEditable(
        model->isDefaultCurrentTimeStampEnabled(index.row()));
    defaultEditor->setOnUpdCurTsEditable(
        model->isDefaultOnUpdCurTsEnabled(index.row()));

    defaultEditor->setDefaultText(model->defaultText(index.row()));
    defaultEditor->setDefaultValue(model->defaultType(index.row()));
}

void TableColumnDefaultDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto defaultEditor = static_cast<ui::TableColumnDefaultEditor *>(editor);
    auto columnsModel = static_cast<models::TableColumnsModel *>(model);

    columnsModel->setDefaultValue(index.row(),
                                  defaultEditor->defaultValue(),
                                  defaultEditor->defaultText());
}

void TableColumnDefaultDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);

    auto tableWidget = static_cast<const QTableView * const>(option.widget);
    int headerWidth = tableWidget->verticalHeader()->width();
    int headerHeight = tableWidget->horizontalHeader()->height();

    QPoint topLeft = tableWidget->mapToGlobal(QPoint(headerWidth, headerHeight))
            + option.rect.topLeft();

    QRect rect = QRect(topLeft, editor->minimumSize());

    // Check if we feet screen and adjust like eg in combobox
    ui::helpers::fitRectToScreen(rect);

    editor->setGeometry(rect);
}

} // namespace delegates
} // namespace ui
} // namespace meow
