#ifndef UI_CENTRAL_RIGHT_VIEWTAB_H
#define UI_CENTRAL_RIGHT_VIEWTAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/common/sql_editor.h"
#include "models/forms/view_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class ViewTab : public BaseRootTab
{
public:
    explicit ViewTab(QWidget * parent = nullptr);

    void setView(const db::ViewEntityPtr & view);

    void onBeforeEntityEditing();

private:
    void createWidgets();
    void createGeneralButtons();
    void fillDataFromForm();

    Q_SLOT void discardViewEditing();
    Q_SLOT void saveViewEditing();

    Q_SLOT void validateControls();

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _definerLabel;
    QComboBox * _definerComboBox;

    QLabel * _algorithmLabel;
    QComboBox * _algorithmComboBox;

    QLabel * _securityLabel;
    QComboBox * _securityComboBox;

    QLabel * _checkOptionLabel;
    QComboBox * _checkOptionComboBox;

    QLabel * _selectLabel;
    ui::common::SQLEditor * _selectEdit;

    QPushButton * _discardButton;
    QPushButton * _saveButton;

    models::forms::ViewForm _form;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_VIEWTAB_H
