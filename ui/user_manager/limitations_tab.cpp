#include "limitations_tab.h"
#include <limits>

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

    const QMap<db::User::LimitType, int> limits = _form->userLimits();

    auto it = limits.constBegin();
    while (it != limits.constEnd()) {

        db::User::LimitType limitType = it.key();

        QLabel * label = new QLabel(_form->limitName(limitType));
        mainGridLayout->addWidget(label, row, 0);

        QSpinBox * editor = new QSpinBox();
        editor->setMinimum(-1); // for PG
        editor->setMaximum(INT_MAX);
        label->setBuddy(editor);
        _limitEditors[limitType] = editor;
        mainGridLayout->addWidget(editor, row, 1);

        ++it;
        ++row;
    }

    mainGridLayout->setColumnMinimumWidth(0, 100);
    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 3);
    mainGridLayout->setAlignment(Qt::AlignTop);

    this->setLayout(mainGridLayout);
}

} // namespace user_manager
} // namespace ui
} // namespace meow
