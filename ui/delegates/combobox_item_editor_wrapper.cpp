#include "combobox_item_editor_wrapper.h"
#include <QComboBox>
#include <QDebug>

namespace meow {
namespace ui {
namespace delegates {

QWidget * ComboboxItemEditorWrapper::createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox * combobox = new QComboBox(parent);
    combobox->setEditable(true); // TODO as property?

    _editor = combobox;

    return _editor;
}

void ComboboxItemEditorWrapper::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{

    auto comboBox = static_cast<QComboBox *>(editor);

    QVariant editData = index.model()->data(index, Qt::EditRole);

    // user data => name
    using IdValueList = QList<QPair<QString, QString>>;

    if (editData.canConvert<IdValueList>()) {

        const IdValueList editDataList = qvariant_cast<IdValueList>(editData);

        auto it = editDataList.constBegin();
        while (it != editDataList.constEnd()) {
             comboBox->addItem(it->second,
                               QVariant(it->first));
             ++it;
        }
        QString value =
                index.model()->data(index, Qt::DisplayRole).toString();

        comboBox->setCurrentIndex(comboBox->findData(value));
        comboBox->setMinimumWidth(180);

    } else if (editData.canConvert<QStringList>()) {
        QStringList values = editData.toStringList();
        comboBox->addItems(values);

        QString value =
                index.model()->data(index, Qt::DisplayRole).toString();
        int valueIndex = comboBox->findText(value);
        if (valueIndex != -1) {
            comboBox->setCurrentIndex(valueIndex);
        } else {
            comboBox->setEditText(value);
        }
    }


}

void ComboboxItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    auto combobox = static_cast<QComboBox *>(editor);

    QString currentText = combobox->currentText();

    int currentIndex = combobox->currentIndex();

    if (currentIndex != -1) {

        QString itemText = combobox->itemText(currentIndex);

        if (currentText != itemText) {
            // User entered custom text => use custom text
            model->setData(index, currentText, Qt::EditRole);
            return;
        }

        QVariant data = combobox->itemData(currentIndex);
        if (data.type() != QVariant::Invalid) {
            // User not entered data, but we have item data => use item data
            model->setData(index, data, Qt::EditRole);
            return;
        }
    }

    // no other options => use current text
    model->setData(index, currentText, Qt::EditRole);
}


} // namespace delegates
} // namespace ui
} // namespace meow
