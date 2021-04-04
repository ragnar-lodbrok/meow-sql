#include "privileges_widget.h"
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

PrivilegesWidget::PrivilegesWidget(models::forms::UserManagementForm * form,
                                   QWidget * parent)
    : QWidget(parent)
    , _treeModel(form)
    , _form(form)
{
    createWidgets();
}

void PrivilegesWidget::fillDataFromForm()
{
    _treeModel.setUser(_form->selectedUser());
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

}

} // namespace user_manager
} // namespace ui
} // namespace meow
