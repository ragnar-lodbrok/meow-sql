#include "table_index_delegate.h"
#include "models/forms/table_indexes_model.h"
#include <QComboBox>

namespace meow {
namespace models {
namespace delegates {

using Cols = models::forms::TableIndexesModel::Columns;

TableIndexDelegate::TableIndexDelegate(QObject * parent)
    :QStyledItemDelegate(parent)
{

}

QWidget * TableIndexDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{

    auto model = static_cast<const models::forms::TableIndexesModel *>(
        index.model());

    Cols column = static_cast<Cols>(index.column());

    if (model->isRowTypeIndex(index)) {
        if (column == Cols::Type || column == Cols::Algorithm) {
            return new QComboBox(parent);
        }
    } else if (model->isRowTypeColumn(index)) {
        if (column == Cols::Name) {
            return new QComboBox(parent);
        }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TableIndexDelegate::setEditorData(
        QWidget *editor,
        const QModelIndex &index) const
{

    auto model = static_cast<const models::forms::TableIndexesModel *>(
        index.model());

    Cols column = static_cast<Cols>(index.column());

    if (model->isRowTypeIndex(index)) {
        if (column == Cols::Type) {
            auto comboBox = static_cast<QComboBox *>(editor);
            comboBox->addItems(model->indexTypes());
            QString value = model->data(index, Qt::EditRole).toString();
            comboBox->setCurrentIndex(comboBox->findText(value));
        } else if (column == Cols::Algorithm) {
            auto comboBox = static_cast<QComboBox *>(editor);
            comboBox->addItem("");
            comboBox->addItems(model->indexAlgorithms());
            QString value = model->data(index, Qt::EditRole).toString();
            comboBox->setCurrentIndex(comboBox->findText(value));
        }
    } else if (model->isRowTypeColumn(index)) {
        if (column == Cols::Name) {
            auto comboBox = static_cast<QComboBox *>(editor);
            comboBox->addItems(model->indexColumns());
            QString value = model->data(index, Qt::EditRole).toString();
            comboBox->setCurrentIndex(comboBox->findText(value));
        }
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void TableIndexDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{

    auto indxModel = static_cast<models::forms::TableIndexesModel *>(model);

    Cols column = static_cast<Cols>(index.column());

    bool isCombobox = false;

    if (indxModel->isRowTypeIndex(index)) {
        isCombobox = (column == Cols::Type) || (column == Cols::Algorithm);
    } else if (indxModel->isRowTypeColumn(index)) {
        isCombobox = (column == Cols::Name);
    }

    if (isCombobox) {
        auto comboBox = static_cast<QComboBox *>(editor);
        QVariant curData = comboBox->currentText();
        indxModel->setData(index, curData, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void TableIndexDelegate::updateEditorGeometry(
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
