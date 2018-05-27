#ifndef FOREIGN_KEY_REFERENCE_OPTION_DELEGATE_H
#define FOREIGN_KEY_REFERENCE_OPTION_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace models {
namespace delegates {

class ForeignKeyReferenceOptionDelegate : public QStyledItemDelegate
{
public:
    ForeignKeyReferenceOptionDelegate(QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

} // namespace delegates
} // namespace models
} // namespace meow

#endif // FOREIGN_KEY_REFERENCE_OPTION_DELEGATE_H
