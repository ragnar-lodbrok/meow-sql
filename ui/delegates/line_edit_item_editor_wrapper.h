#ifndef UI_DELEGATES_LINE_EDIT_QUERY_DATA_DELEGATE_H
#define UI_DELEGATES_LINE_EDIT_QUERY_DATA_DELEGATE_H

#include "edit_query_data_delegate.h"

namespace meow {
namespace ui {
namespace delegates {

// TODO: rm one Edit?
class LineEditItemEditorWrapper : public ItemEditorWrapper
{
    Q_OBJECT
public:
    LineEditItemEditorWrapper(EditQueryDataDelegate * delegate)
        : ItemEditorWrapper(delegate) {

    }

    virtual QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const override;

    virtual void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:

    Q_SLOT void onPopupTextEditorClosed(bool accepted);

};

} // namespace delegates
} // namespace ui
} // namespace meow


#endif // UI_DELEGATES_LINE_EDIT_QUERY_DATA_DELEGATE_H
