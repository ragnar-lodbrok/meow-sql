#include "limitations_tab.h"
#include <limits>
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

LimitationsTab::LimitationsTab(models::forms::UserManagementForm * form,
                               QWidget * parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}

void LimitationsTab::createWidgets()
{
    QGridLayout * mainGridLayout = new QGridLayout();

    int row = 0;

    using LimitType = meow::db::User::LimitType;
    const QList<LimitType> limits = _form->supportedLimitTypes();

    for (const LimitType limitType : limits) {

        QLabel * label = new QLabel(_form->limitName(limitType));
        _limitLabels[limitType] = label;
        mainGridLayout->addWidget(label, row, 0);

        QSpinBox * editor = new QSpinBox();
        editor->setMinimum(-1); // for PG
        editor->setMaximum(INT_MAX);
        label->setBuddy(editor);
        _limitEditors[limitType] = editor;
        mainGridLayout->addWidget(editor, row, 1);

        ++row;
    }

    mainGridLayout->setColumnMinimumWidth(0, 100);
    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 3);
    mainGridLayout->setAlignment(Qt::AlignTop);

    this->setLayout(mainGridLayout);
}

void LimitationsTab::fillDataFromForm()
{
    auto it = _limitEditors.constBegin();
    while (it != _limitEditors.constEnd()) {

        db::User::LimitType limitType = it.key();

        QSpinBox * editor = it.value();
        QLabel * label = _limitLabels[limitType];

        label->setEnabled(_form->hasUser());
        editor->setEnabled(_form->hasUser());

        editor->blockSignals(true);
        editor->setValue(_form->limitValue(limitType));
        editor->blockSignals(false);

        ++it;
    }
}

} // namespace user_manager
} // namespace ui
} // namespace meow
