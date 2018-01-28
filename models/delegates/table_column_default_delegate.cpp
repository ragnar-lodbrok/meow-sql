#include "table_column_default_delegate.h"
#include "ui/common/table_column_default_editor.h"
#include <QDebug>

static QRect screenRectForRect(const QRect & rect)
{
    auto screens = QApplication::screens();

    for (const auto & screen : screens) {
        auto screenRect = screen->virtualGeometry();
        if (screenRect.intersects(rect)) {
            return screen->availableGeometry();
        }
    }
    return QRect();
}

static void fitRectToRect(QRect & rectSmall, const QRect & rectBig)
{
    if (rectSmall.width() > rectBig.width() ||
        rectSmall.height() > rectBig.height() ) {
        QSize boundedSize = rectSmall.size().boundedTo(rectBig.size());
        rectSmall.setSize(boundedSize);
    }

    if (rectSmall.bottomRight().x() > rectBig.bottomRight().x() ||
        rectSmall.bottomRight().y() > rectBig.bottomRight().y() ) {

        rectSmall.translate(
            rectBig.bottomRight() - rectBig.united(rectSmall).bottomRight()
        );
    } else if (rectSmall.topLeft().x() < rectBig.topLeft().x() ||
               rectSmall.topLeft().y() < rectBig.topLeft().y()) {
        rectSmall.translate(
            rectBig.topLeft() - rectBig.united(rectSmall).topLeft()
        );
    }
}

namespace meow {
namespace models {
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

    Q_UNUSED(editor);
    Q_UNUSED(index);

    //auto comboBox = static_cast<ui::DataTypeComboBox *>(editor);
    //comboBox->fillData();
    //QString value = index.model()->data(index, Qt::EditRole).toString();
    //comboBox->setCurrentIndex(comboBox->findText(value));
}

void TableColumnDefaultDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{

    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);

    //auto comboBox = static_cast<ui::DataTypeComboBox *>(editor);
    //QVariant curData = comboBox->currentData();
    //model->setData(index, curData, Qt::EditRole);
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
    QRect screenRect = screenRectForRect(rect);
    if (screenRect.contains(rect) == false) {

        fitRectToRect(rect, screenRect);
    }

    editor->setGeometry(rect);
}

} // namespace delegates
} // namespace models
} // namespace meow
