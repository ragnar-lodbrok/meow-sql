#ifndef UI_DELEGATES_COMBOBOX_DELEGATE_H
#define UI_DELEGATES_COMBOBOX_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace ui {
namespace delegates {

class ComboboxDelegate : public QStyledItemDelegate
{
public:
    ComboboxDelegate(QObject * parent = nullptr);

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

    void setListDataRole(Qt::ItemDataRole role) {
        _listDataRole = role;
    }
    void setCurrentValueDataRole(Qt::ItemDataRole role) {
        _currentValueDataRole = role;
    }
    void setComboboxEditable(bool editable) {
        _isComboboxEditable = editable;
    }
private:
    Qt::ItemDataRole _listDataRole;
    Qt::ItemDataRole _currentValueDataRole;
    bool _isComboboxEditable;
};

} // namespace delegates
} // namespace ui
} // namespace meow

#endif // UI_DELEGATES_COMBOBOX_DELEGATE_H
