#ifndef UI_DELEGATES_DATE_TIME_ITEM_EDITOR_WRAPPER_H
#define UI_DELEGATES_DATE_TIME_ITEM_EDITOR_WRAPPER_H

#include "edit_query_data_delegate.h"

namespace meow {
namespace ui {
namespace delegates {

class DateTimeItemEditorWrapper : public ItemEditorWrapper
{
public:

    enum class Format
    {
        DateTime,
        Date,
        Time,
        Year
    };

    DateTimeItemEditorWrapper(EditQueryDataDelegate * delegate,
                              Format format = Format::DateTime)
        : ItemEditorWrapper(delegate)
        , _format(format)
    {

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
    Format _format;
};

} // namespace delegates
} // namespace ui
} // namespace meow


#endif // UI_DELEGATES_DATE_TIME_ITEM_EDITOR_WRAPPER_H
