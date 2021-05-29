#include "privileges_widget.h"
#include "models/forms/user_management_form.h"
#include "ui/user_manager/select_db_object.h"
#include "db/exception.h"
#include "user_manager_window.h"

namespace meow {
namespace ui {
namespace user_manager {

PrivilegesWidget::PrivilegesWidget(models::forms::UserManagementForm * form,
                                   Window * window)
    : QWidget(window)
    , _treeModel(form)
    , _form(form)
    , _window(window)
{
    createWidgets();
}

void PrivilegesWidget::fillDataFromForm()
{
    _treeModel.setUser(_form->selectedUser());
}

void PrivilegesWidget::validateControls()
{
    _addObjectButton->setEnabled(_form->canAddObject());
}

void PrivilegesWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    QHBoxLayout * addObjectLayout = new QHBoxLayout();
    addObjectLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(addObjectLayout);

    _accessLabel = new QLabel(tr("Allow access to:"));
    addObjectLayout->addWidget(_accessLabel, 0);

    _addObjectButton = new QPushButton(QIcon(":/icons/add.png"),
                                       tr("Add object"));
    _addObjectButton->setFlat(true);
    addObjectLayout->addWidget(_addObjectButton, 0, Qt::AlignRight);
    connect(_addObjectButton,
            &QAbstractButton::clicked,
            this,
            &PrivilegesWidget::onAddObjectClicked
    );

    _privilegesTree = new QTreeView();
    _privilegesTree->setHeaderHidden(true);
    _privilegesTree->setModel(&_treeModel);

    mainLayout->addWidget(_privilegesTree, 1);
}

void PrivilegesWidget::onAddObjectClicked()
{
    SelectDbObject dialog(_form->session());
    if (dialog.exec() == QDialog::Accepted) {
        const models::forms::SelectDBObjectForm::Object & object
                = dialog.form()->currentSelection();
        try {
            bool added = _treeModel.appendPrivilegeObject(
                        object.type,
                        object.databaseName,
                        object.entityName,
                        object.fieldName);
            if (added) {
                _form->setHasUnsavedChanges(true);
            }
        } catch (const meow::db::Exception & ex) {
            _window->showErrorMessage(ex.message());
        }

    }

}

} // namespace user_manager
} // namespace ui
} // namespace meow
