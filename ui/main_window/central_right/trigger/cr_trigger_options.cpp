#include "cr_trigger_options.h"
#include "models/forms/trigger_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TriggerOptions::TriggerOptions(models::forms::TriggerForm * form,
                               QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
    validateControls();
}

void TriggerOptions::refreshData()
{
    fillDataFromForm();
    validateControls();
}

void TriggerOptions::createWidgets()
{
    QGridLayout * mainLayout = new QGridLayout();

    // Name + Definer row
    _nameLabel = new QLabel(tr("Name:"));
    _nameEdit = new QLineEdit();
    _nameLabel->setBuddy(_nameEdit);
    connect(_nameEdit, &QLineEdit::textEdited,
            [=](const QString &name) {
                if (!_form->isEditingSupported()) return;
                _form->setName(name);
            });

    _definerLabel = new QLabel(tr("Definer:"));
    _definerCombobox = new QComboBox();
    _definerLabel->setBuddy(_definerCombobox);
    connect(_definerCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & definer) {
                if (!_form->isEditingSupported()) {
                    _definerCombobox->setCurrentIndex(
                        _definerCombobox->findText(_form->definer())
                    );
                    return;
                }
                _form->setDefiner(definer);
            });

    // Table

    _tableLabel = new QLabel(tr("On table:"));
    _tableCombobox = new QComboBox();
    _tableLabel->setBuddy(_tableCombobox);
    connect(_tableCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & tableName) {
                if (!_form->isEditingSupported()) {
                    _tableCombobox->setCurrentIndex(
                        _tableCombobox->findText(_form->tableName())
                    );
                    return;
                }
                _form->setTableName(tableName);
            });

    // Event

    _eventLabel = new QLabel(tr("Event:"));

    _actionTimeCombobox = new QComboBox();
    connect(_actionTimeCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & actionTime) {
                if (!_form->isEditingSupported()) {
                    _actionTimeCombobox->setCurrentIndex(
                        _actionTimeCombobox->findText(_form->actionTime())
                    );
                    return;
                }
                _form->setActionTime(actionTime);
            });

    _eventCombobox = new QComboBox();
    connect(_eventCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & event) {
                if (!_form->isEditingSupported()) {
                    _eventCombobox->setCurrentIndex(
                        _eventCombobox->findText(_form->eventName())
                    );
                    return;
                }
                _form->setEventName(event);
            });

    QHBoxLayout * eventLayout = new QHBoxLayout();

    eventLayout->addWidget(_actionTimeCombobox, 2);
    eventLayout->addSpacing(8);
    eventLayout->addWidget(_eventCombobox, 2);
    eventLayout->addStretch(3);


    // ------------------------------

    int row = 0, col = 0;

    mainLayout->addWidget(_nameLabel, row, col++);
    mainLayout->addWidget(_nameEdit, row, col++);
    mainLayout->addWidget(_definerLabel, row, col++);
    mainLayout->addWidget(_definerCombobox, row, col);

    ++row; col = 0;

    mainLayout->addWidget(_tableLabel, row, col++);
    mainLayout->addWidget(_tableCombobox, row, col);

    ++row; col = 0;

    mainLayout->addWidget(_eventLabel, row, col++);
    mainLayout->addLayout(eventLayout, row, col, 1, 3);

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 4);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->setColumnStretch(3, 6);

    mainLayout->setHorizontalSpacing(15);
    mainLayout->setAlignment(Qt::AlignTop);

    setLayout(mainLayout);
}

void TriggerOptions::fillDataFromForm()
{
    _nameEdit->blockSignals(true);
    _nameEdit->setText(_form->name());
    _nameEdit->setReadOnly(!_form->isEditingSupported());
    _nameEdit->blockSignals(false);

    _definerCombobox->blockSignals(true);
    _definerCombobox->clear();
    _definerCombobox->addItems(
        _form->allDefinerOptions(_form->isEditingSupported())
     ); // TODO: lazy load
    _definerCombobox->setCurrentIndex(
        _definerCombobox->findText(_form->definer()));
    _definerCombobox->setEditable(_form->isEditingSupported());
    _definerCombobox->blockSignals(false);

    _tableCombobox->blockSignals(true);
    _tableCombobox->clear();
    _tableCombobox->addItems(_form->allTableNameOptions());
    _tableCombobox->setCurrentIndex(
        _tableCombobox->findText(_form->tableName()));
    _tableCombobox->blockSignals(false);

    _actionTimeCombobox->blockSignals(true);
    _actionTimeCombobox->clear();
    _actionTimeCombobox->addItems(_form->allActionTimeOptions());
    _actionTimeCombobox->setCurrentIndex(
        _actionTimeCombobox->findText(_form->actionTime()));
    _actionTimeCombobox->blockSignals(false);

    _eventCombobox->blockSignals(true);
    _eventCombobox->clear();
    _eventCombobox->addItems(_form->allEventOptions());
    _eventCombobox->setCurrentIndex(
        _eventCombobox->findText(_form->eventName()));
    _eventCombobox->blockSignals(false);
}

void TriggerOptions::validateControls()
{
    _definerLabel->setEnabled(_form->supportsDefiner());
    _definerCombobox->setEnabled(_form->supportsDefiner());
}

void TriggerOptions::onBeforeEntityEditing()
{
    _nameEdit->setFocus();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
