#include "editable_query_data_table_view.h"
#include "ui/presenters/editable_data_context_menu_presenter.h"

#include <QtWidgets>

namespace meow {
namespace ui {

EditableQueryDataTableView::EditableQueryDataTableView(
        models::BaseDataTableModel * model,
        QWidget * parent)
    : TableView(parent)
    , _model(model)
{
    Q_ASSERT(_model != nullptr);
}

void EditableQueryDataTableView::contextMenuEvent(QContextMenuEvent * event)
{

    presenters::EditableDataContextMenuPresenter presenter(_model);

    QMenu menu(this);

    if (presenter.supportsInsertValue()) {

        QMenu * insertValueSubMenu = menu.addMenu( // owns result
            QIcon(":/icons/calendar_view_day.png"),
            tr("Insert value"));

        insertValueSubMenu->addAction(presenter.setNullAction());

        std::vector<QAction *> dateTimeActions = presenter.setDateTimeActions();

        if (!dateTimeActions.empty()) {
            insertValueSubMenu->addSeparator();
        }

        for (QAction * dateTimeAction : dateTimeActions) {
            dateTimeAction->setParent(insertValueSubMenu);

            insertValueSubMenu->addAction(dateTimeAction);

            connect(dateTimeAction,
                    &QAction::triggered,
                    [=](){
                        QModelIndex curIndex = selectionModel()->currentIndex();
                        model()->setData(curIndex, dateTimeAction->data());
                    }
            );

            insertValueSubMenu->addAction(dateTimeAction);
        }

        menu.addSeparator();
    }

    std::vector<QAction *> editRowActions = presenter.editRowActions();
    if (!editRowActions.empty()) {

        for (QAction * editRowAction : editRowActions) {
            menu.addAction(editRowAction);
        }

        menu.addSeparator();
    }

    if (presenter.resetDataSortAction()) {
        menu.addAction(presenter.resetDataSortAction());
    }

    if (presenter.exportDataAction()) {
        menu.addAction(presenter.exportDataAction());
    }

    if (presenter.refreshDataAction()) {
        menu.addAction(presenter.refreshDataAction());
    }

    menu.exec(event->globalPos());
}

} // namespace ui
} // namespace meow
