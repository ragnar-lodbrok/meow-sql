#ifndef UI_DELEGATES_CHECKBOX_DELEGATE_H
#define UI_DELEGATES_CHECKBOX_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace ui {
namespace delegates {

class CheckboxDelegate : public QStyledItemDelegate
{
public:
    CheckboxDelegate(QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    bool editorEvent(QEvent * event,
                     QAbstractItemModel * model,
                     const QStyleOptionViewItem & option,
                     const QModelIndex & index) override;

    void paint(QPainter *painter,
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

private:

    bool isChecked(const QModelIndex &index) const;
    bool isEnabled(const QModelIndex &index) const;

};

} // namespace delegates
} // namespace ui
} // namespace meow

#endif // UI_DELEGATES_CHECKBOX_DELEGATE_H
