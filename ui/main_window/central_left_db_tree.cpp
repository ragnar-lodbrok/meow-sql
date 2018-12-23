#include "central_left_db_tree.h"
#include <QtWidgets>
#include "models/db/entities_tree_model.h"
#include "app/app.h"
#include "helpers/logger.h"

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
    // TODO: enablement

    menu.addAction(meow::app()->actions()->disconnect());
    menu.addAction(meow::app()->actions()->sessionManager());

    menu.exec(event->globalPos());
}

void DbTree::createActions()
{
    // drop ====================================================================
    _dropAction = new QAction(QIcon(":/icons/application_form_delete.png"),
                              tr("Drop ..."), this);
    _dropAction->setStatusTip(
        tr("Deletes tables, views, procedures and functions"));
    connect(_dropAction, &QAction::triggered,  [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());

        db::Entity * currentEntity = treeModel->currentEntity();
        if (!currentEntity) return;

        QString confirmMsg;
        QMessageBox::Icon msgIcon = QMessageBox::Question;

        if (currentEntity->type() == db::Entity::Type::Database) {

            confirmMsg = QObject::tr("Drop Database \"%1\"?")
                            .arg(currentEntity->name());

            confirmMsg += "\n\n";
            confirmMsg += QObject::tr("WARNING: You will lose all objects "
                                      "in database %1!")
                            .arg(currentEntity->name());

            msgIcon = QMessageBox::Critical;
        } else {
            confirmMsg = QObject::tr("Drop %1 object(s) in database \"%2\"?")
                            .arg(1)
                            .arg(db::databaseName(currentEntity));
            confirmMsg += "\n\n" + currentEntity->name();

        }

        QMessageBox msgBox;
        msgBox.setText(confirmMsg);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);
        int ret = msgBox.exec();
        if (ret != QMessageBox::Yes) {
            return;
        }

        try {
            treeModel->dropCurrentItem();
        } catch(meow::db::Exception & ex) {
            QMessageBox msgBox;
            msgBox.setText(ex.message());
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(msgIcon);
            msgBox.exec();
        }

    });

    // create table ============================================================
    _createTableAction = new QAction(QIcon(":/icons/table.png"),
                                     tr("Table"), this);
    _createTableAction->setStatusTip(
               tr("Create new table in selected database"));
    connect(_createTableAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());
        treeModel->createNewTable();
    });


    // refresh =================================================================
    _refreshAction = new QAction(QIcon(":/icons/arrow_refresh.png"),
                                 tr("Refresh"), this);
    _refreshAction->setShortcuts(QKeySequence::Refresh);
    connect(_refreshAction, &QAction::triggered, [=](bool checked) {
        meowLogDebug() << "refresh";
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());

        try {
            treeModel->refreshActiveSession();
        } catch(meow::db::Exception & ex) {
            QMessageBox msgBox;
            msgBox.setText(ex.message());
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    });

    // disconnect ==============================================================
}

} // namespace meow
} // namespace ui
} // namespace main_window
