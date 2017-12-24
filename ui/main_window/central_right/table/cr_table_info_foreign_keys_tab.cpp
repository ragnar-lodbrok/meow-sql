#include "cr_table_info_foreign_keys_tab.h"
#include "models/forms/table_info_form.h"
#include "models/forms/table_foreign_keys_model.h"
#include "app.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

ForeignKeysTab::ForeignKeysTab(models::forms::TableInfoForm * form,
                               QWidget *parent)
    : QWidget(parent),
      _tableForm(form)
{
    Q_CHECK_PTR(form);
    createWidgets();
}

void ForeignKeysTab::createWidgets()
{
    // TODO: show a message when db doesn't support FK

    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(_mainLayout);

    _fKeysTable = new QTableView();
    _fKeysTable->verticalHeader()->hide();
     auto geometrySettings = meow::app()->settings()->geometrySettings();
    _fKeysTable->verticalHeader()->setDefaultSectionSize(
        geometrySettings->tableViewDefaultRowHeight());
    _fKeysTable->horizontalHeader()->setHighlightSections(false);

    auto model = _tableForm->foreignKeysModel();
    _fKeysTable->setModel(model);
    for (int i = 0; i < model->columnCount(); ++i) {
        _fKeysTable->setColumnWidth(i, model->columnWidth(i));
    }
    _mainLayout->addWidget(_fKeysTable);
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
