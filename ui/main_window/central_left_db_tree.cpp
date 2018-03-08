#include "central_left_db_tree.h"
#include <QtWidgets>
#include "models/db/entities_tree_model.h"

namespace meow {
namespace ui {
namespace main_window {

DbTree::DbTree(QWidget * parent) : QTreeView(parent)
{
    createActions();
}

void DbTree::contextMenuEvent(QContextMenuEvent * event)
{
    auto treeModel = static_cast<models::db::EntitiesTreeModel *>( model() );

    QMenu menu(this);

    _dropAction->setEnabled(treeModel->canDropCurrentItem());
    menu.addAction(_dropAction);

    QMenu * createSubMenu = menu.addMenu( // owns result
        QIcon(":/icons/application_form_add.png"),
        tr("Create new"));
    _createTableAction->setEnabled(treeModel->canInsertTableOnCurrentItem());
    createSubMenu->addAction(_createTableAction);

    menu.addSeparator();

    menu.addAction(_refreshAction);

    menu.exec(event->globalPos());
}

void DbTree::createActions()
{
    // drop ====================================================================
    _dropAction = new QAction(QIcon(":/icons/application_form_delete.png"),
                              tr("Drop ..."), this);
    _dropAction->setStatusTip(
        tr("Deletes tables, views, procedures and functions"));
    //connect(_dropAction, &QAction::triggered, this, &DbTree::onDropAction);



    // create table ============================================================
    _createTableAction = new QAction(QIcon(":/icons/table.png"),
                                     tr("Table"), this);
    _createTableAction->setStatusTip(
               tr("Create new table in selected database"));
    connect(_createTableAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>( model() );
        treeModel->createNewTable();
    });


    // refresh =================================================================
    _refreshAction = new QAction(QIcon(":/icons/arrow_refresh.png"),
                                 tr("Refresh"), this);
    _refreshAction->setShortcuts(QKeySequence::Refresh);
}

} // namespace meow
} // namespace ui
} // namespace main_window
