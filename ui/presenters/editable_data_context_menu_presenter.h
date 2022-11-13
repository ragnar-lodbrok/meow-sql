#ifndef UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H
#define UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H

#include <QAction>
#include <vector>

namespace meow {
namespace ui {

namespace models {
class BaseDataTableModel;
}

namespace presenters {

class EditableDataContextMenuPresenter
{
public:
    EditableDataContextMenuPresenter(models::BaseDataTableModel * model);

    bool supportsInsertValue() const;

    QAction * setNullAction() const;

    QAction * refreshDataAction() const;

    std::vector<QAction *> setDateTimeActions() const;

    std::vector<QAction *> editRowActions() const;

    QAction * resetDataSortAction() const;

    QAction * exportDataAction() const;
private:

    bool isTableModel() const;
    bool isQueryModel() const;

    models::BaseDataTableModel * _model;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_PRESENTERS_EDITABLE_DATA_CONTEXT_MENU_PRESENTER_H

