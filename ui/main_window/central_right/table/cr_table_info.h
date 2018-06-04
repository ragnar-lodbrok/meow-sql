#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_H

#include <QtWidgets>
#include "models/ui/table_info_widget_model.h"

namespace meow {

namespace models {
namespace forms {
    class TableInfoForm;
}
}

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
    Q_OBJECT

public:
    explicit TableInfo(models::forms::TableInfoForm * form,
                       QWidget *parent = nullptr);

    void refreshData();

    void onBeforeEntityEditing();

    virtual QSize sizeHint() const override {
        return QSize(200, 200);
    }

    Q_SIGNAL void indicesModified();
    Q_SIGNAL void foreignKeysModified();

private:
    void createTabs();

    models::ui::TableInfoWidgetModel _model;
    models::forms::TableInfoForm * _form;

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
