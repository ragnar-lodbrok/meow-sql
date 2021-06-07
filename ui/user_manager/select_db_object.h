#ifndef UI_USER_MANAGER_SELECT_DB_OBJECT_H
#define UI_USER_MANAGER_SELECT_DB_OBJECT_H

#include <QtWidgets>
#include "models/db/session_objects_tree_model.h"
#include "models/forms/select_db_object_form.h"

namespace meow {
namespace ui {
namespace user_manager {

class SelectDbObject : public QDialog
{
public:
    SelectDbObject(meow::db::SessionEntity * session);

    const models::forms::SelectDBObjectForm * form() const {
        return &_form;
    }
private:
    void createWidgets();
    void validateControls();

    Q_SLOT void selectedObjectsChanged(
        const QItemSelection &selected,
        const QItemSelection &deselected);

    Q_SLOT void showErrorMessage(const QString& message);
    Q_SLOT void databaseNameChanged(const QString& name);

    QLabel * _selectLabel;
    QTreeView * _objectsTree;
    QLabel * _wildcardLabel;
    QLineEdit * _databaseEdit;

    QDialogButtonBox * _buttonBox;
    QPushButton * _okButton;
    QPushButton * _cancelButton;

    models::db::SessionObjectsTreeModel _treeModel;
    models::forms::SelectDBObjectForm _form;
};

} // namespace user_manager
} // namespace ui
} // namespace meow


#endif // UI_USER_MANAGER_SELECT_DB_OBJECT_H