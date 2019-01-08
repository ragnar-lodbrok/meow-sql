#include "dialog.h"

namespace meow {
namespace ui {
namespace edit_database {

Dialog::Dialog() : QDialog(nullptr, Qt::WindowCloseButtonHint)
{
    setMinimumSize(320, 200);
    setMaximumSize(600, 400);
    setWindowTitle(tr("Create database ..."));

    createWidgets();

    resize(380, 180);
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
    _nameLabel->setBuddy(_nameEdit);
    mainLayout->addWidget(_nameEdit, row, 1);

    ++row;

    // Collation ---------------------------------------------------------------
    _collationLabel = new QLabel(tr("Collation:"));
    mainLayout->addWidget(_collationLabel, row, 0);

    _collationComboBox = new QComboBox;
    _collationLabel->setBuddy(_collationComboBox);
    mainLayout->addWidget(_collationComboBox, row, 1);

    ++row;

    // Buttons -----------------------------------------------------------------
    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(_buttonBox, row, 0, 1, 2, Qt::AlignBottom);
    mainLayout->setRowStretch(row, 2);

    ++row;
}

} // namespace edit_database
} // namespace ui
} // namespace meow

