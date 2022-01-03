#include "select_db_object.h"
#include "db/exception.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace user_manager {

SelectDbObject::SelectDbObject(meow::db::SessionEntity * session)
    : QDialog(nullptr, Qt::WindowCloseButtonHint)
    , _form(session)
{

    createWidgets();
    validateControls();

    setMinimumSize(QSize(400, 440));
    resize(minimumSize());
    setWindowTitle(tr("Select database object ..."));

    connect(_objectsTree->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &SelectDbObject::selectedObjectsChanged
    );

    connect(&_treeModel,
            &models::SessionObjectsTreeModel::fetchObjectError,
            this,
            &SelectDbObject::showErrorMessage
    );

    connect(&_form,
            &presenters::SelectDBObjectForm::databasenameChanged,
            this,
            &SelectDbObject::databaseNameChanged
    );

    try {
        _treeModel.setSession(session);
        _objectsTree->expandToDepth(0); // show databases
    } catch(meow::db::Exception & ex) {
        showErrorMessage(ex.what());
    }
}

void SelectDbObject::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    _selectLabel = new QLabel(tr("Select database, table or column:"));
    mainLayout->addWidget(_selectLabel, 0);

    _objectsTree = new QTreeView();
    _objectsTree->setHeaderHidden(true);
    _objectsTree->setModel(&_treeModel);
    mainLayout->addWidget(_objectsTree, 1);

    _wildcardLabel = new QLabel(tr(
                                "... or wildcard database (% and _ allowed):"));
    mainLayout->addWidget(_wildcardLabel, 0);

    _databaseEdit = new QLineEdit();
    connect(_databaseEdit, &QLineEdit::textEdited,
            [=](const QString &databaseName) {
                _form.setDatabaseWildcard(databaseName);
            });
    mainLayout->addWidget(_databaseEdit, 0);

    _buttonBox = new QDialogButtonBox();

    _okButton = _buttonBox->addButton(QDialogButtonBox::Ok);
    _cancelButton = _buttonBox->addButton(QDialogButtonBox::Cancel);

    connect(_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(_buttonBox, 0);
}

void SelectDbObject::validateControls()
{
    _okButton->setEnabled(_form.hasValidSelection());
}

void SelectDbObject::selectedObjectsChanged(
        const QItemSelection &selected,
        const QItemSelection &deselected)
{

    Q_UNUSED(deselected);

    QModelIndex index;
    QModelIndexList items = selected.indexes();

    if (!items.isEmpty()) {
        index = items.at(0);
    }

    models::SessionObjectsTreeModel::Object treeObject
            = _treeModel.objectAt(index);

    _form.setTreeSelection({
                            treeObject.type,
                            treeObject.databaseName,
                            treeObject.entityName,
                            treeObject.fieldName
                            });

    validateControls();
}

void SelectDbObject::databaseNameChanged(const QString& name)
{
    if (_databaseEdit->text() != name) {
        _databaseEdit->setText(name); // avoid cursor jump
    }
    validateControls();
}

void SelectDbObject::showErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

} // namespace user_manager
} // namespace ui
} // namespace meow
