#include "checkbox_delegate.h"
#include <QCheckBox>
#include <QApplication>
#include <QDebug>
#include <QPainter>

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
    // normally is not called, kept just in case
    Q_UNUSED(option);
    Q_UNUSED(index);

    return new QCheckBox(parent);
}

void CheckboxDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{
    // NU
    auto checkBox = static_cast<QCheckBox *>(editor);
    checkBox->setChecked(this->isChecked(index));
}

bool CheckboxDelegate::editorEvent(QEvent * event,
                 QAbstractItemModel * model,
                 const QStyleOptionViewItem & option,
                 const QModelIndex & index)
{
    // Using QCheckBox the cell is editable after click (so you click twice)
    // but it's better to switch checkbox state on single click,
    // so we dont't use editor
    // Model should return Qt::ItemIsEnabled if checkbox may be changed
    // Qt::ItemIsEditable should be false to avoid triggering standard editor

    if (!this->isEnabled(index)) {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    if (event->type() == QEvent::MouseButtonRelease) {

        bool isChecked = this->isChecked(index);

        Qt::CheckState checkState = (isChecked ? Qt::Unchecked : Qt::Checked);

        model->setData(index, checkState, Qt::EditRole);

        return true;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void CheckboxDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& opt,
           const QModelIndex& index) const
{

    // TODO: not sure this paint() is correct, check this one:
    // https://github.com/pierreet/BooleanCheckBoxDelegate/blob/master/BooleanCheckBoxDelegate.h#L47

    if (!this->isEnabled(index)) {
        // TODO: paint disabled
        //QStyledItemDelegate::paint(painter, opt, index);
        return;
    }

    // TODO: paint focus, press

    bool checked = this->isChecked(index);

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

    checkbox.state |= QStyle::State_Enabled;

    // the checkbox floats in the top left corner of table in mac os
    // this bug is still present in Qt 5.9.8, but may be solved in Qt 5.11
    // See https://bugreports.qt.io/browse/QTBUG-40833
    #ifdef Q_OS_MAC
        painter->save();
        painter->translate(checkbox.rect.topLeft());
    #endif

    QApplication::style()->drawControl(QStyle::CE_CheckBox,
                                       &checkbox,
                                       painter);
    #ifdef Q_OS_MAC
        painter->restore();
    #endif
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

bool CheckboxDelegate::isChecked(const QModelIndex &index) const
{
    int intChecked = index.model()->data(index, Qt::CheckStateRole).toInt();
    return static_cast<Qt::CheckState>(intChecked) == Qt::Checked;
}

bool CheckboxDelegate::isEnabled(const QModelIndex &index) const
{
    return (index.model()->flags(index) & Qt::ItemIsEnabled);
}

} // namespace delegates
} // namespace models
} // namespace meow
