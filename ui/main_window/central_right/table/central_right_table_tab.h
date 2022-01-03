#ifndef UI_CENTRAL_RIGHT_TABLETAB_H
#define UI_CENTRAL_RIGHT_TABLETAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/presenters/table_info_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TableInfo;
class TableColumns;

class TableTab : public BaseRootTab
{
    Q_OBJECT
public:

    explicit TableTab(QWidget * parent = nullptr);
    ~TableTab();

    void setTable(const db::TableEntityPtr & table);

    void onBeforeEntityEditing();
private:

    void saveGeometryToSettings();
    void loadGeometryFromSettings();

    void createWidgets();
    void createGeneralButtons();

    void validateControls();

    Q_SLOT void discardTableEditing();
    Q_SLOT void saveTableEditing();

    presenters::TableInfoForm _form; // put it somewere in db layer?

    QSplitter * _mainSplitter;

    TableInfo * _tableInfo;
    TableColumns * _columns;

    QPushButton * _discardButton;
    QPushButton * _saveButton;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLETAB_H
