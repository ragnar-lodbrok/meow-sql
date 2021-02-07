#include "central_right_trigger_tab.h"
#include "cr_trigger_options.h"
#include "cr_trigger_body.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TriggerTab::TriggerTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();

    connect(&_form,
            &models::forms::TriggerForm::unsavedChanged,
            [=](bool hasUnsavedChanges) {
                Q_UNUSED(hasUnsavedChanges);
                validateControls();
            }
    );
}

void TriggerTab::setTrigger(db::TriggerEntity * trigger)
{
    _form.setTrigger(trigger);
    _options->refreshData();
    _body->refreshData();
    validateControls();
}

void TriggerTab::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _options = new TriggerOptions(&_form, this);
    _options->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _options->setMinimumSize(QSize(300, 120));

    _body = new TriggerBody(&_form, this);
    _body->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _body->setMinimumSize(QSize(300, 150));

    _mainSplitter->addWidget(_options);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_body);
    _mainSplitter->setStretchFactor(1, 2);

    createGeneralButtons();
    validateControls();
}

void TriggerTab::createGeneralButtons()
{
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    ((QVBoxLayout *)this->layout())->addLayout(buttonsLayout);

    _discardButton = new QPushButton(tr("Discard"));
    buttonsLayout->addWidget(_discardButton);
    connect(_discardButton,
            &QAbstractButton::clicked,
            this,
            &TriggerTab::discardEditing
    );

    _saveButton = new QPushButton(tr("Save"));
    buttonsLayout->addWidget(_saveButton);
    connect(_saveButton,
            &QAbstractButton::clicked,
            this,
            &TriggerTab::saveEditing
    );

    buttonsLayout->addStretch(1);
}

void TriggerTab::discardEditing()
{
    if (_form.sourceTrigger()) {
        this->setTrigger(_form.sourceTrigger());
    }
}

void TriggerTab::saveEditing()
{
    try {
        _form.setStatement(_body->bodyText());
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

void TriggerTab::validateControls()
{
    bool enableEdit = _form.hasUnsavedChanges() && _form.isEditingSupported();
    _discardButton->setEnabled(enableEdit);
    _saveButton->setEnabled(enableEdit && !_form.name().isEmpty());
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
