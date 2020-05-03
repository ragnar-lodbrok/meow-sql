#include "foreign_key_columns_delegate.h"
#include <QDebug>
#include "models/forms/table_foreign_keys_model.h"
#include "ui/common/checkbox_list_popup.h"
#include "ui/common/geometry_helpers.h"

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

    auto widget = new ui::CheckboxListPopup(parent);

    setEditorData(widget, index); // to calc geometry

    return widget;
}

void ForeignKeyColumnsDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto listView = static_cast<ui::CheckboxListPopup *>(editor);
    auto model = static_cast<const models::forms::TableForeignKeysModel *>(
        index.model());

    const QStringList & allColumns = model->allColumns();
    const QStringList & checkedCols
            = model->data(index, Qt::EditRole).toStringList();

    listView->setData(allColumns, checkedCols);
}

void ForeignKeyColumnsDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{

    auto listWidget = static_cast<ui::CheckboxListPopup *>(editor);

    model->setData(index, listWidget->selectedOptions(), Qt::EditRole);
}

void ForeignKeyColumnsDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{

    Q_UNUSED(index);

    auto editorWidget = static_cast<ui::CheckboxListPopup *>(editor);

    QSize size = option.rect.size();
    size.setHeight(editorWidget->heightHint());
    size.setWidth(std::max(editorWidget->widthHint(), size.width()));

    auto tableWidget = static_cast<const QTableView * const>(option.widget);
    int headerWidth = tableWidget->verticalHeader()->width();
    int headerHeight = tableWidget->horizontalHeader()->height();

    QPoint topLeft = tableWidget->mapToGlobal(QPoint(headerWidth, headerHeight))
            + option.rect.topLeft();

    QRect rect = QRect(topLeft, size);

    // Check if we feet screen and adjust like eg in combobox
    ui::helpers::fitRectToScreen(rect);

    editor->setGeometry(rect);
}

} // namespace delegates
} // namespace models
} // namespace meow
