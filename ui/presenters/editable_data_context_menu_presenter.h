#ifndef UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H
#define UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H

#include <QAction>
#include <vector>

namespace meow {
namespace ui {
namespace presenters {

class EditableDataContextMenuPresenter
{
public:
    EditableDataContextMenuPresenter() { }

    bool supportsInsertValue() const;

    QAction * setNullAction() const;

    QAction * refreshDataAction() const;

    std::vector<QAction *> setDateTimeActions() const;

    std::vector<QAction *> editRowActions() const;

    QAction * resetDataSortAction() const;

    QAction * exportDataAction() const;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H

