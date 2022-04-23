#ifndef UI_CENTRAL_RIGHT_DATA_FILTER_WIDGET_H
#define UI_CENTRAL_RIGHT_DATA_FILTER_WIDGET_H

#include <QtWidgets>
#include "ui/common/sql_log_editor.h"
#include "ui/presenters/central_right_data_filter_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class DataFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataFilterWidget(models::DataTableModel * dataTableModel,
                              QWidget *parent = nullptr);

    void setDBEntity(db::Entity * tableOrViewEntity);

private:
    void createWidgets();
    void validateControls();
    void fillRecentFilters();

    Q_SLOT void onFilterEditChanged(const QString &text);
    Q_SLOT void onSQLFilterTextGenerated(const QString &text);
    Q_SLOT void onSQLFilterTextChanged();
    Q_SLOT void onApplyFilterButtonClicked();
    Q_SLOT void onClearFilterButtonClicked();
    Q_SLOT void onRecentFiltersChanged();
    Q_SLOT void onComboboxRecentFiltersActivated(int index);

    QLabel * _labelRecentFilters;
    QComboBox * _comboboxRecentFilters;
    ui::common::SQLEditor * _sqlEditor;

    QLabel * _labelColumnFilter;
    QLineEdit * _editFilterSearch;
    QPushButton * _applyFilterButton;
    QPushButton * _clearFilterButton;
    QWidget * _recentFiltersWidget;

    ui::presenters::CentralRightDataFilterForm _form;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_DATA_FILTER_WIDGET_H
