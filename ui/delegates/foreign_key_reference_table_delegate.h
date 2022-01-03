#ifndef UI_DELEGATES_FOREIGN_KEY_REFERENCE_TABLE_DELEGATE_H
#define UI_DELEGATES_FOREIGN_KEY_REFERENCE_TABLE_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace ui {
namespace delegates {

class ForeignKeyReferenceTableDelegate : public QStyledItemDelegate
{
public:
    ForeignKeyReferenceTableDelegate(QObject * parent = nullptr);

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
} // namespace ui
} // namespace meow

#endif // UI_DELEGATES_FOREIGN_KEY_REFERENCE_TABLE_DELEGATE_H
