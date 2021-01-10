#include "central_right_routine_tab.h"
#include "cr_routine_body.h"
#include "cr_routine_info.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

RoutineTab::RoutineTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();

    connect(&_form,
            &models::forms::RoutineForm::unsavedChanged,
            [=](bool hasUnsavedChanges) {
                Q_UNUSED(hasUnsavedChanges);
                validateControls();
            }
    );
}

void RoutineTab::setRoutine(meow::db::RoutineEntity * routine)
{
    _form.parametersModel()->setRoutine(nullptr); // TODO: crap

    _form.setRoutine(routine);
    _form.parametersModel()->setRoutine(_form.editableRoutine());
    _info->refreshData();
    _body->refreshData();
    validateControls();
}

void RoutineTab::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _info = new RoutineInfo(&_form, this);
    _info->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _info->setMinimumSize(QSize(300, 200));

    _body = new RoutineBody(&_form, this);
    _body->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _body->setMinimumSize(QSize(300, 150));

    _mainSplitter->addWidget(_info);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_body);
    _mainSplitter->setStretchFactor(1, 2);

    createGeneralButtons();
    validateControls();
}

void RoutineTab::createGeneralButtons()
{
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    ((QVBoxLayout *)this->layout())->addLayout(buttonsLayout);

    _discardButton = new QPushButton(tr("Discard"));
    buttonsLayout->addWidget(_discardButton);
    connect(_discardButton,
            &QAbstractButton::clicked,
            this,
            &RoutineTab::discardEditing
    );

    _saveButton = new QPushButton(tr("Save"));
    buttonsLayout->addWidget(_saveButton);
    connect(_saveButton,
            &QAbstractButton::clicked,
            this,
            &RoutineTab::saveEditing
    );

    buttonsLayout->addStretch(1);
}

void RoutineTab::discardEditing()
{
    this->setRoutine(_form.sourceRoutine());
}

void RoutineTab::saveEditing()
{
    try {
        _form.setBody(_body->bodyText());
        //_form.save();
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void RoutineTab::validateControls()
{
    bool enableEdit = _form.hasUnsavedChanges() && _form.isEditingSupported();
    _discardButton->setEnabled(enableEdit);
    _saveButton->setEnabled(enableEdit && !_form.name().isEmpty());
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
