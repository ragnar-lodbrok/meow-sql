#ifndef UI_USER_MANAGER_OPTIONS_WIDGET_H
#define UI_USER_MANAGER_OPTIONS_WIDGET_H

#include <QtWidgets>
#include "credentials_tab.h"
#include "limitations_tab.h"

namespace meow {
namespace ui {
namespace presenters {

class UserManagementForm;

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

} // namespace presenters

namespace user_manager {

class OptionsWidget : public QWidget
{
public:
    explicit OptionsWidget(presenters::UserManagementForm * form,
                           QWidget * parent = nullptr);
    void fillDataFromForm();
    void onAddUserClicked();
private:

    void createWidgets(presenters::UserManagementForm * form);

    presenters::UserOptionsWidgetModel _model;

    QTabWidget  * _rootTabs;
    CredentialsTab * _credentialsTab;
    LimitationsTab * _limitationsTab;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_OPTIONS_WIDGET_H
