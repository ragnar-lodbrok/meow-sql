#ifndef SESSIONMANAGERSTARTTAB_H
#define SESSIONMANAGERSTARTTAB_H

#include <QtWidgets>

#include "models/forms/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class StartTab : public QWidget
{
    Q_OBJECT
public:
    explicit StartTab(QWidget *parent = 0);
    void setForm(models::forms::ConnectionParametersForm * form);
private:

    void fillDataFromForm();

    QVBoxLayout * _mainLayout;
    QLabel * _helpLabel;
    QPushButton * _importSettingsButton;

    models::forms::ConnectionParametersForm * _form;
};

} // namespace session_manager
} // namespace ui
} // namespace meow

#endif // SESSIONMANAGERSTARTTAB_H
