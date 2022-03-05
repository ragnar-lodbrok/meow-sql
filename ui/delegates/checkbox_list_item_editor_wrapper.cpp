#include "checkbox_list_item_editor_wrapper.h"
#include "ui/common/checkbox_list_popup.h"
#include "ui/common/geometry_helpers.h"

namespace meow {
namespace ui {
namespace delegates {

QWidget * CheckboxListItemEditorWrapper::createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    auto widget = new ui::CheckboxListPopup(parent);

    setEditorData(widget, index); // to calc correct geometry

    _editor = widget;

    return _editor;
}

void CheckboxListItemEditorWrapper::setEditorData(QWidget *editor,
                                const QModelIndex &index) const
{

    auto listView = static_cast<ui::CheckboxListPopup *>(editor);

    const QStringList allColumns
            = index.model()->data(index, Qt::EditRole).toStringList();
    const QStringList checkedCols
            = index.model()->data(index, Qt::DisplayRole).toString().split(',');

    listView->setData(allColumns, checkedCols);
}

void CheckboxListItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    auto listWidget = static_cast<ui::CheckboxListPopup *>(editor);

    model->setData(index,
                   listWidget->selectedOptions().join(','),
                   Qt::EditRole);
}

void CheckboxListItemEditorWrapper::updateEditorGeometry(QWidget *editor,
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
} // namespace ui
} // namespace meow

