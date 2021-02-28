#ifndef UI_USER_MANAGER_OPTIONS_WIDGET_H
#define UI_USER_MANAGER_OPTIONS_WIDGET_H

#include <QtWidgets>
#include "credentials_tab.h"
#include "limitations_tab.h"

namespace meow {
namespace models {

namespace forms {
    class UserManagementForm;
} // namespace forms

namespace ui {

class UserOptionsWidgetModel
{
public:

    enum class Tabs {
        Credentials,
        Limitations
    };

    const QString tabTitle(Tabs tab) const {
        switch (tab) {
        case Tabs::Credentials:
            return QObject::tr("Credentials");
        case Tabs::Limitations:
            return QObject::tr("Limitations");
        default:
            Q_ASSERT(false);
            return QString();
        }
    }
};

} // namespace ui
} // namespace models
} // namespace meow

namespace meow {
namespace ui {
namespace user_manager {

class OptionsWidget : public QWidget
{
public:
    explicit OptionsWidget(models::forms::UserManagementForm * form,
                           QWidget * parent = nullptr);
private:

    void createWidgets(models::forms::UserManagementForm * form);

    models::ui::UserOptionsWidgetModel _model;

    QTabWidget  * _rootTabs;
    CredentialsTab * _credentialsTab;
    LimitationsTab * _limitationsTab;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_OPTIONS_WIDGET_H
