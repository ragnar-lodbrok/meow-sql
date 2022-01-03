#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_H

#include <QtWidgets>
#include "ui/presenters/table_info_widget_model.h"

namespace meow {
namespace ui {
namespace presenters {
    class TableInfoForm;
}
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
    explicit TableInfo(presenters::TableInfoForm * form,
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

    presenters::TableInfoWidgetModel _model;
    presenters::TableInfoForm * _form;

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
