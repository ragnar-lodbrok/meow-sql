#include "checkbox_delegate.h"
#include <QCheckBox>
#include <QApplication>
#include <QDebug>

namespace meow {
namespace models {
namespace delegates {

CheckboxDelegate::CheckboxDelegate(QObject * parent)
    :QStyledItemDelegate(parent)
{

}

QWidget * CheckboxDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new QCheckBox(parent);
}

void CheckboxDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    auto checkBox = static_cast<QCheckBox *>(editor);
    int intChecked = index.model()->data(index, Qt::EditRole).toInt();
    if (static_cast<Qt::CheckState>(intChecked) == Qt::Checked) {
        checkBox->setChecked(true);
    } else {
        checkBox->setChecked(false);
    }
}

void CheckboxDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& opt,
           const QModelIndex& index) const
{

    bool enabled = (index.model()->flags(index) & Qt::ItemIsEnabled);
    if (!enabled) {
        // TODO: paint disabled
        //QStyledItemDelegate::paint(painter, opt, index);
        return;
    }

    int intChecked = index.model()->data(index, Qt::CheckStateRole).toInt();
    bool checked = (static_cast<Qt::CheckState>(intChecked) == Qt::Checked);

    QStyleOptionButton checkbox;

    QSize checkboxSize = QApplication::style()->sizeFromContents(
        QStyle::CT_CheckBox,
        &checkbox,
        QSize());

    int newXMiddle = (opt.rect.width() - checkboxSize.width()) / 2;

    checkbox.rect = opt.rect;
    checkbox.rect.adjust(newXMiddle, 0, -newXMiddle, 0);

    if (checked) {
        checkbox.state |= QStyle::State_On;
    } else {
        checkbox.state |= QStyle::State_Off;
    }

    QApplication::style()->drawControl(QStyle::CE_CheckBox,
                                       &checkbox,
                                       painter);
}

void CheckboxDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    auto checkBox = static_cast<QCheckBox *>(editor);;
    QVariant curData = checkBox->isChecked();
    model->setData(index, curData, Qt::EditRole);
}

void CheckboxDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);

    QStyleOptionButton checkbox;
    checkbox.rect = option.rect;

    QSize checkboxSize = QApplication::style()->sizeFromContents(
        QStyle::CT_CheckBox,
        &checkbox,
        QSize());

    int newXMiddle = (checkbox.rect.width() - checkboxSize.width()) / 2;
    checkbox.rect.adjust(newXMiddle, 0, -newXMiddle, 0);

    editor->setGeometry(checkbox.rect);
}

} // namespace delegates
} // namespace models
} // namespace meow
