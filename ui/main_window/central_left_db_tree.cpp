#include "central_left_db_tree.h"
#include <QtWidgets>
#include "models/db/entities_tree_model.h"
#include "app/app.h"
#include "helpers/logger.h"
#include "db/entity/database_entity.h"

#include "ui/edit_database/dialog.h"
#include "models/forms/edit_database_form.h"

#include "ui/export_database/export_dialog.h"
#include "models/forms/export_database_form.h"

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

    // edit

    if (currentItemSupportsEditing()) {
        menu.addAction(_editAction);
    }

    // drop

    _dropAction->setEnabled(treeModel->canDropCurrentItem());
    menu.addAction(_dropAction);

    // create

    QMenu * createSubMenu = menu.addMenu( // owns result
        QIcon(":/icons/application_form_add.png"),
        tr("Create new"));

    _createDatabaseAction->setEnabled(
        treeModel->canCreateDatabaseOnCurrentItem());
    createSubMenu->addAction(_createDatabaseAction);

    _createTableAction->setEnabled(
            treeModel->canCreateEntityOnCurrentItem(
                    meow::db::Entity::Type::Table));
    createSubMenu->addAction(_createTableAction);

    _createViewAction->setEnabled(
            treeModel->canCreateEntityOnCurrentItem(
                    meow::db::Entity::Type::View));
    createSubMenu->addAction(_createViewAction);

    _createRoutineAction->setEnabled(
            treeModel->canCreateEntityOnCurrentItem(
                    meow::db::Entity::Type::Procedure));
    createSubMenu->addAction(_createRoutineAction);

    _createTriggerAction->setEnabled(
            treeModel->canCreateEntityOnCurrentItem(
                    meow::db::Entity::Type::Trigger));
    createSubMenu->addAction(_createTriggerAction);

    // ------------------
    menu.addSeparator();

    if (currentItemSupportsDumping()) {
        menu.addAction(meow::app()->actions()->exportDatabase());
    }


    menu.addSeparator();

    menu.addAction(_refreshAction);
    // TODO: enablement

    menu.addAction(meow::app()->actions()->disconnect());
    menu.addAction(meow::app()->actions()->sessionManager());

    menu.exec(event->globalPos());
}

void DbTree::refresh()
{
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
}

void DbTree::createActions()
{
    // edit ====================================================================

    _editAction = new QAction(QIcon(":/icons/application_form_edit.png"),
                              tr("Edit"), this);
    _editAction->setStatusTip(tr("Edit selected object"));

    connect(_editAction, &QAction::triggered,  [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel =
        static_cast<models::db::EntitiesTreeModel *>(model());

        db::Entity * currentEntity = treeModel->currentEntity();
        if (!currentEntity
                || currentEntity->type() != db::Entity::Type::Database) {
            return;
        }
        auto database = static_cast<db::DataBaseEntity *>(currentEntity);

        db::SessionEntity * session
            = treeModel->dbConnectionsManager()->activeSession();
        models::forms::EditDatabaseForm form(session, database, false);
        meow::ui::edit_database::Dialog dialog(&form);
        dialog.exec();

        if (form.dropOldDatabase()) {
            treeModel->dropEntity(database);
        }
    });

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

    // create database =========================================================

    _createDatabaseAction = new QAction(QIcon(":/icons/database.png"),
                                     tr("Database"), this);
    _createDatabaseAction->setStatusTip(tr("Create a new, blank database"));
    connect(_createDatabaseAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel =
        static_cast<models::db::EntitiesTreeModel *>(model());

        db::SessionEntity * session
            = treeModel->dbConnectionsManager()->activeSession();
        models::forms::EditDatabaseForm form(session, nullptr, true);
        meow::ui::edit_database::Dialog dialog(&form);
        dialog.exec();
    });


    // create table ============================================================
    _createTableAction = new QAction(QIcon(":/icons/table.png"),
                                     tr("Table"), this);
    _createTableAction->setStatusTip(
               tr("Create new table in selected database"));
    connect(_createTableAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());
        treeModel->createNewEntity(meow::db::Entity::Type::Table);
    });

    // create view =============================================================
    _createViewAction = new QAction(QIcon(":/icons/view.png"),
                                     tr("View"), this);
    _createViewAction->setStatusTip(
               tr("Create view ..."));
    connect(_createViewAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());
        treeModel->createNewEntity(meow::db::Entity::Type::View);
    });

    // create routine ==========================================================
    _createRoutineAction = new QAction(QIcon(":/icons/stored_procedure.png"),
                                     tr("Stored routine"), this);
    _createRoutineAction->setStatusTip(
               tr("Create stored procedure or function"));
    connect(_createRoutineAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());
        treeModel->createNewEntity(meow::db::Entity::Type::Procedure);
    });

    // create trigger ==========================================================
    _createTriggerAction = new QAction(QIcon(":/icons/trigger.png"),
                                     tr("Trigger"), this);
    _createTriggerAction->setStatusTip(
               tr("Create a trigger"));
    connect(_createTriggerAction, &QAction::triggered, [=](bool checked){
        Q_UNUSED(checked);
        auto treeModel = static_cast<models::db::EntitiesTreeModel *>(model());
        treeModel->createNewEntity(meow::db::Entity::Type::Trigger);
    });

    // export ==================================================================

    connect(meow::app()->actions()->exportDatabase(),
            &QAction::triggered,
            [=](bool checked)
    {
        Q_UNUSED(checked);
        auto treeModel =
        static_cast<models::db::EntitiesTreeModel *>(model());

        db::SessionEntity * session
            = treeModel->dbConnectionsManager()->activeSession();

        models::forms::ExportDatabaseForm form(session);

        db::Entity * currentEntity = treeModel->currentEntity();
        if (currentEntity) {
            db::Entity * db = meow::db::findParentEntityOfType(
                        currentEntity,
                        meow::db::Entity::Type::Database);
            if (db) {
                form.setDatabase(db->name());
                form.setAllDatabases(false);
            }
        }

        meow::ui::export_database::Dialog dialog(&form);
        dialog.exec();
    });

    // refresh =================================================================
    _refreshAction = new QAction(QIcon(":/icons/arrow_refresh.png"),
                                 tr("Refresh"), this);
    _refreshAction->setShortcuts(QKeySequence::Refresh);
    connect(_refreshAction, &QAction::triggered, [=](bool checked) {

        Q_UNUSED(checked);
        refresh();
    });

}

bool DbTree::currentItemSupportsDumping() const
{
    auto treeModel =
    static_cast<models::db::EntitiesTreeModel *>(model());

    db::Entity * currentEntity = treeModel->currentEntity();
    if (currentEntity) {
        return currentEntity->connection()->features()->supportsDumping();
    }

    return false;
}

bool DbTree::currentItemSupportsEditing() const
{
    auto treeModel =
    static_cast<models::db::EntitiesTreeModel *>(model());

    db::Entity * currentEntity = treeModel->currentEntity();
    if (currentEntity) {

        if (currentEntity->type() == db::Entity::Type::Database) {
            return currentEntity->connection()
                    ->features()->supportsEditingDatabase();
        }
        // TODO: other types
    }

    return false;
}

} // namespace meow
} // namespace ui
} // namespace main_window
