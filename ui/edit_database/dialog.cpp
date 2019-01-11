#include "dialog.h"
#include "models/forms/edit_database_form.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace edit_database {

Dialog::Dialog(models::forms::EditDatabaseForm * form)
    : QDialog(nullptr, Qt::WindowCloseButtonHint),
      _form(form)
{
    setMinimumSize(320, 200);
    setMaximumSize(600, 400);
    setWindowTitle(tr("Create database ..."));

    createWidgets();
    fillDataFromForm();

    resize(380, 150);
}

void Dialog::createWidgets()
{
    QGridLayout * mainLayout = new QGridLayout();
    this->setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 2);

    int row = 0;

    // Name --------------------------------------------------------------------
    _nameLabel = new QLabel(tr("Name:"));
    mainLayout->addWidget(_nameLabel, row, 0);

    _nameEdit = new QLineEdit();
    _nameEdit->setMaxLength(64);
    // TODO: input mask?
    connect(_nameEdit, &QLineEdit::textEdited,
            [=](const QString &name) {
                _form->setName(name);
                validate();
            });

    _nameLabel->setBuddy(_nameEdit);
    mainLayout->addWidget(_nameEdit, row, 1);

    ++row;

    // Collation ---------------------------------------------------------------
    _collationLabel = new QLabel(tr("Collation:"));
    mainLayout->addWidget(_collationLabel, row, 0);

    _collationComboBox = new QComboBox;
    connect(_collationComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                _form->setCollation(text);
                validate();
            });
    _collationLabel->setBuddy(_collationComboBox);
    mainLayout->addWidget(_collationComboBox, row, 1);

    ++row;

    // Buttons -----------------------------------------------------------------
    _buttonBox = new QDialogButtonBox();

    _okButton = _buttonBox->addButton(QDialogButtonBox::Ok);
    _cancelButton = _buttonBox->addButton(QDialogButtonBox::Cancel);

    connect(_buttonBox, &QDialogButtonBox::accepted,
            this, &Dialog::onAccept);
    connect(_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(_buttonBox, row, 0, 1, 2, Qt::AlignBottom);
    mainLayout->setRowStretch(row, 2);

    ++row;
}

void Dialog::fillDataFromForm()
{
    _nameEdit->blockSignals(true);
    _nameEdit->setText(_form->name());
    _nameEdit->blockSignals(false);

    _collationComboBox->blockSignals(true);
    _collationComboBox->clear();
    _collationComboBox->addItems(_form->collations());
    _collationComboBox->setCurrentIndex(
        _collationComboBox->findText(_form->collation()));
    _collationComboBox->blockSignals(false);

    validate();
}

void Dialog::validate()
{
    _okButton->setEnabled(_form->canSave());
}

void Dialog::onAccept()
{
    try {

        _form->save();

    } catch(meow::db::Exception & ex) {

        QString message = QString(tr("Creating database '%1' failed.\n%2")).arg(
            _form->name(),
            ex.message()
        );

        QMessageBox msgBox;
        msgBox.setText(message);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        return;
    }

    accept();
}

} // namespace edit_database
} // namespace ui
} // namespace meow

