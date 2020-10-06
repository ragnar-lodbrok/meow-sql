#include "central_right_view_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

ViewTab::ViewTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void ViewTab::createWidgets()
{

    QGridLayout * mainGridLayout = new QGridLayout();

    // name ------------------------------

    _nameLabel = new QLabel(tr("Name:"));
    mainGridLayout->addWidget(_nameLabel, 0, 0);

    _nameEdit = new QLineEdit();
    _nameLabel->setBuddy(_nameEdit);
    mainGridLayout->addWidget(_nameEdit, 0, 1);

    // definer ---------------------------

    _definerLabel = new QLabel(tr("Definer:"));
    mainGridLayout->addWidget(_definerLabel, 0, 2);

    _definerComboBox = new QComboBox();
    _definerLabel->setBuddy(_definerComboBox);
    connect(_definerComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                Q_UNUSED(text);
                if (!_form.isEditingSupported()) {
                    _definerComboBox->setCurrentIndex(
                        _definerComboBox->findText(_form.definer()));
                    return;
                }
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
            });
    mainGridLayout->addWidget(_checkOptionComboBox, 2, 3);

    // select ----------------------------

    _selectLabel = new QLabel(tr("Select statement:"));
    mainGridLayout->addWidget(_selectLabel, 3, 0, 1, 4);

    _selectEdit = new ui::common::SQLEditor();
    mainGridLayout->addWidget(_selectEdit, 4, 0, 1, 4);

    // -----------------------------------

    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 2);
    mainGridLayout->setColumnStretch(2, 0);
    mainGridLayout->setColumnStretch(3, 2);

    mainGridLayout->setAlignment(Qt::AlignTop);
    this->setLayout(mainGridLayout);
}

void ViewTab::fillDataFromForm()
{
    _nameEdit->blockSignals(true);
    _nameEdit->setText(_form.name());
    _nameEdit->setReadOnly(!_form.isEditingSupported());
    _nameEdit->blockSignals(false);

    _definerComboBox->blockSignals(true);
    _definerComboBox->clear();
    _definerComboBox->addItems(_form.allDefinerOptions());
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
}

void ViewTab::setView(db::ViewEntity * view)
{
    _form.setView(view);
    fillDataFromForm();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
