#include "central_right_view_tab.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

ViewTab::ViewTab(QWidget * parent)
    : BaseRootTab(BaseRootTab::Type::Entity, parent)
{
    createWidgets();

    connect(&_form,
            &models::forms::ViewForm::unsavedChanged,
            [=](bool hasUnsavedChanges) {
                Q_UNUSED(hasUnsavedChanges);
                validateControls();
            }
    );
}

void ViewTab::createWidgets()
{

    QGridLayout * mainGridLayout = new QGridLayout();

    // name ------------------------------

    _nameLabel = new QLabel(tr("Name:"));
    mainGridLayout->addWidget(_nameLabel, 0, 0);

    _nameEdit = new QLineEdit();
    _nameLabel->setBuddy(_nameEdit);
    _nameEdit->setMaxLength(64);
    connect(_nameEdit, &QLineEdit::textEdited,
            [=](const QString &name) {
                _form.setName(name);
                validateControls();
            });
    mainGridLayout->addWidget(_nameEdit, 0, 1);

    // definer ---------------------------

    _definerLabel = new QLabel(tr("Definer:"));
    mainGridLayout->addWidget(_definerLabel, 0, 2);

    _definerComboBox = new QComboBox();
    _definerLabel->setBuddy(_definerComboBox);
    // TODO: free editing
    connect(_definerComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                Q_UNUSED(text);
                if (!_form.isEditingSupported()) {
                    _definerComboBox->setCurrentIndex(
                        _definerComboBox->findText(_form.definer()));
                    return;
                }
                _form.setDefiner(text);
            });
    mainGridLayout->addWidget(_definerComboBox, 0, 3);

    // algorithm -------------------------

    _algorithmLabel = new QLabel(tr("Algorithm:"));
    mainGridLayout->addWidget(_algorithmLabel, 1, 0);

    _algorithmComboBox = new QComboBox();
    _algorithmLabel->setBuddy(_algorithmComboBox);
    connect(_algorithmComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                Q_UNUSED(text);
                if (!_form.isEditingSupported()) {
                    _algorithmComboBox->setCurrentIndex(
                        _algorithmComboBox->findText(_form.algorithm()));
                    return;
                }
                _form.setAlgorithm(text);
            });
    mainGridLayout->addWidget(_algorithmComboBox, 1, 1);

    // security -------------------------

    _securityLabel = new QLabel(tr("SQL security:"));
    mainGridLayout->addWidget(_securityLabel, 1, 2);

    _securityComboBox = new QComboBox();
    _securityLabel->setBuddy(_securityComboBox);
    connect(_securityComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                Q_UNUSED(text);
                if (!_form.isEditingSupported()) {
                    _securityComboBox->setCurrentIndex(
                        _securityComboBox->findText(_form.security()));
                    return;
                }
                _form.setSecurity(text);
            });
    mainGridLayout->addWidget(_securityComboBox, 1, 3);

    // check option ---------------------

    _checkOptionLabel = new QLabel(tr("Check option for updates:"));
    mainGridLayout->addWidget(_checkOptionLabel, 2, 2);

    _checkOptionComboBox = new QComboBox();
    _checkOptionLabel->setBuddy(_checkOptionComboBox);
    connect(_checkOptionComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                Q_UNUSED(text);
                if (!_form.isEditingSupported()) {
                    _checkOptionComboBox->setCurrentIndex(
                        _checkOptionComboBox->findText(_form.checkOption()));
                    return;
                }
                _form.setCheckOption(text);
            });
    mainGridLayout->addWidget(_checkOptionComboBox, 2, 3);

    // select ----------------------------

    _selectLabel = new QLabel(tr("Select statement:"));
    mainGridLayout->addWidget(_selectLabel, 3, 0, 1, 4);

    _selectEdit = new ui::common::SQLEditor();
    connect(_selectEdit, &QPlainTextEdit::textChanged,
            [=]() {
                _form.setHasUnsavedChanges(true);
                // don't copy each time as can be long
            });
    mainGridLayout->addWidget(_selectEdit, 4, 0, 1, 4);

    // -----------------------------------

    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 2);
    mainGridLayout->setColumnStretch(2, 0);
    mainGridLayout->setColumnStretch(3, 2);

    mainGridLayout->setAlignment(Qt::AlignTop);
    this->setLayout(mainGridLayout);

    createGeneralButtons();
    validateControls();
}

void ViewTab::createGeneralButtons()
{
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    ((QGridLayout *)this->layout())->addLayout(buttonsLayout, 5, 0, 1, 4);

    _discardButton = new QPushButton(tr("Discard"));
    buttonsLayout->addWidget(_discardButton);
    connect(_discardButton,
            &QAbstractButton::clicked,
            this,
            &ViewTab::discardViewEditing
    );

    _saveButton = new QPushButton(tr("Save"));
    buttonsLayout->addWidget(_saveButton);
    connect(_saveButton,
            &QAbstractButton::clicked,
            this,
            &ViewTab::saveViewEditing
    );

    buttonsLayout->addStretch(1);
}

void ViewTab::fillDataFromForm()
{
    _nameEdit->blockSignals(true);
    _nameEdit->setText(_form.name());
    _nameEdit->setReadOnly(!_form.isEditingSupported());
    _nameEdit->blockSignals(false);

    _definerComboBox->blockSignals(true);
    _definerComboBox->clear();
    _definerComboBox->addItems(
        _form.allDefinerOptions(_form.isEditingSupported())
     ); // TODO: lazy load
    _definerComboBox->setCurrentIndex(
        _definerComboBox->findText(_form.definer()));
    _definerComboBox->setEnabled(_form.supportsDefiner());
    _definerComboBox->blockSignals(false);

    _algorithmComboBox->blockSignals(true);
    _algorithmComboBox->clear();
    _algorithmComboBox->addItems(_form.algorithmOptions());
    _algorithmComboBox->setCurrentIndex(
        _algorithmComboBox->findText(_form.algorithm()));
    _algorithmComboBox->setEnabled(_form.supportsAlgorithm());
    _algorithmComboBox->blockSignals(false);

    _securityComboBox->blockSignals(true);
    _securityComboBox->clear();
    _securityComboBox->addItems(_form.securityOptions());
    _securityComboBox->setCurrentIndex(
        _securityComboBox->findText(_form.security()));
    _securityComboBox->setEnabled(_form.supportsSecurity());
    _securityComboBox->blockSignals(false);

    _checkOptionComboBox->blockSignals(true);
    _checkOptionComboBox->clear();
    _checkOptionComboBox->addItems(_form.checkOptions());
    _checkOptionComboBox->setCurrentIndex(
        _checkOptionComboBox->findText(_form.checkOption()));
    _checkOptionComboBox->setEnabled(_form.supportsSecurity());
    _checkOptionComboBox->blockSignals(false);

    _selectEdit->blockSignals(true);
    _selectEdit->setPlainText(_form.selectStatement());
    _selectEdit->setReadOnly(!_form.isEditingSupported());
    _selectEdit->blockSignals(false);

    _discardButton->setVisible(_form.isEditingSupported());
    _saveButton->setVisible(_form.isEditingSupported());
}

void ViewTab::discardViewEditing()
{
    if (_form.sourceView()) {
        this->setView(_form.sourceView());
    }
}

void ViewTab::saveViewEditing()
{
    try {
        _form.setSelectStatement(_selectEdit->toPlainText());
        _form.save();
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void ViewTab::validateControls()
{
    bool enableEdit = _form.hasUnsavedChanges() && _form.isEditingSupported();
    _discardButton->setEnabled(enableEdit);
    _saveButton->setEnabled(enableEdit && !_form.name().isEmpty());
}

void ViewTab::setView(const db::ViewEntityPtr & view)
{
    _form.setView(view);
    fillDataFromForm();
}

void ViewTab::onBeforeEntityEditing()
{
    _nameEdit->setFocus();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
