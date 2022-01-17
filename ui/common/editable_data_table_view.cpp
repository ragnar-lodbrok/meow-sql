#include "editable_data_table_view.h"
#include "ui/presenters/editable_data_context_menu_presenter.h"

#include <QtWidgets>

namespace meow {
namespace ui {

void EditableDataTableView::contextMenuEvent(QContextMenuEvent * event)
{

    presenters::EditableDataContextMenuPresenter presenter;

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
    }

    menu.addSeparator();

    for (QAction * editRowAction : presenter.editRowActions()) {
        menu.addAction(editRowAction);
    }

    menu.addSeparator();

    menu.addAction(presenter.refreshDataAction());

    menu.exec(event->globalPos());
}

} // namespace ui
} // namespace meow
