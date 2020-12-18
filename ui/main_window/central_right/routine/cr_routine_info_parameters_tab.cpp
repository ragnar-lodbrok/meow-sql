#include "cr_routine_info_parameters_tab.h"
#include "app/app.h"
#include "models/forms/routine_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace routine_info {


ParametersTab::ParametersTab(models::forms::RoutineForm *form,
                             QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}


void ParametersTab::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(mainLayout);

    // TODO: tools

    _paramsTable = new QTableView();
    _paramsTable->verticalHeader()->hide();
     auto geometrySettings = meow::app()->settings()->geometrySettings();
    _paramsTable->verticalHeader()->setDefaultSectionSize(
        geometrySettings->tableViewDefaultRowHeight());
    _paramsTable->horizontalHeader()->setHighlightSections(false);

    auto model = _form->parametersModel();
    _paramsTable->setModel(model);
    for (int i = 0; i < model->columnCount(); ++i) {
        _paramsTable->setColumnWidth(i, model->columnWidth(i));
    }

    _paramsTable->setSelectionBehavior(
          QAbstractItemView::SelectionBehavior::SelectRows);
    _paramsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    mainLayout->addWidget(_paramsTable, 1);
}

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
