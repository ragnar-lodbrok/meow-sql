#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_H

#include <QtWidgets>
#include "models/ui/table_info_widget_model.h"
#include "models/forms/table_info_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

namespace table_info {
    class BasicTab;
    class OptionsTab;
    class IndexesTab;
    class ForeignKeysTab;
}

class TableInfo : public QWidget
{
public:
    explicit TableInfo(QWidget *parent = nullptr);

    void setTable(db::TableEntity * table);

    virtual QSize sizeHint() const override {
        return QSize(200, 200);
    }

private:
    void createTabs();

    models::ui::TableInfoWidgetModel _model;
    models::forms::TableInfoForm _form; // put it somewere in db layer?

    QTabWidget  * _rootTabs;
    table_info::BasicTab * _basicTab;
    table_info::OptionsTab * _optionsTab;
    table_info::IndexesTab * _indexesTab;
    table_info::ForeignKeysTab * _fKeysTab;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_H
