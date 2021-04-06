#ifndef UI_USER_MANAGER_LIMITATIONS_TAB_H
#define UI_USER_MANAGER_LIMITATIONS_TAB_H

#include <QtWidgets>
#include "models/forms/user_management_form.h"

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models

namespace ui {
namespace user_manager {

class LimitationsTab : public QWidget
{
public:
    LimitationsTab(models::forms::UserManagementForm * form,
                   QWidget * parent = nullptr);
    void fillDataFromForm();
private:

    void createWidgets();
    Q_SLOT void onLimitSpinBoxValueChanged(int value);

    QMap<meow::db::User::LimitType, QLabel *> _limitLabels;
    QMap<meow::db::User::LimitType, QSpinBox *> _limitEditors;

    models::forms::UserManagementForm * _form;
};

} // namespace user_manager
} // namespace ui
} // namespace meow


#endif // UI_USER_MANAGER_LIMITATIONS_TAB_H
