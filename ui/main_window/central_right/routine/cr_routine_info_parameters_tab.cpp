#include "cr_routine_info_parameters_tab.h"
#include "cr_routine_parameters_tools.h"
#include "app/app.h"
#include "models/forms/routine_form.h"
#include "models/delegates/combobox_delegate.h"

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

    _tools = new RoutineParametersTools(this);
    mainLayout->addWidget(_tools, 0);

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

    connect(_paramsTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &ParametersTab::currentRowChanged
    );

    models::delegates::ComboboxDelegate * contextDelegate
        = new models::delegates::ComboboxDelegate(model);
    _paramsTable->setItemDelegateForColumn(
        (int)models::db::RoutineParametersModel::Columns::Context,
        contextDelegate);

    models::delegates::ComboboxDelegate * dataTypeDelegate
        = new models::delegates::ComboboxDelegate(model);
    dataTypeDelegate->setComboboxEditable(true);
    _paramsTable->setItemDelegateForColumn(
        (int)models::db::RoutineParametersModel::Columns::DataType,
        dataTypeDelegate);

    mainLayout->addWidget(_paramsTable, 1);
}

void ParametersTab::refreshData()
{
    validateControls();
}

void ParametersTab::validateControls()
{
    models::db::RoutineParametersModel * model = _form->parametersModel();

    using ParamAction = RoutineParametersTools::Action;

    QModelIndex curIndex = _paramsTable->selectionModel()->currentIndex();

    _tools->setActionEnabled(ParamAction::Add,
                             model->canAddParam());
    _tools->setActionEnabled(ParamAction::Clear,
                             model->canRemoveAllParams());
    _tools->setActionEnabled(ParamAction::Remove,
                             model->canRemoveParam(curIndex));
    _tools->setActionEnabled(ParamAction::MoveUp,
                             model->canMoveParamUp(curIndex));
    _tools->setActionEnabled(ParamAction::MoveDown,
                             model->canMoveParamDown(curIndex));
}

void ParametersTab::onAddAction()
{
    // Listening: Rage Of Light - Fallen

    models::db::RoutineParametersModel * model = _form->parametersModel();

    // insert
    QModelIndex curIndex = _paramsTable->selectionModel()->currentIndex();
    int curIntIndex = curIndex.isValid() ? curIndex.row() : -1;
    int newRowIntIndex = model->insertEmptyDefaultParam(curIntIndex);

    // select
    QModelIndex newRowNameIndex = _paramsTable->model()->index(
        newRowIntIndex,
        (int)meow::models::db::RoutineParametersModel::Columns::Name);
    _paramsTable->selectionModel()
        ->setCurrentIndex(newRowNameIndex, QItemSelectionModel::Current);

    _paramsTable->selectionModel()
        ->select(newRowNameIndex, QItemSelectionModel::ClearAndSelect);

    // scroll
    _paramsTable->scrollTo(_paramsTable->selectionModel()->currentIndex());

    // edit name
    _paramsTable->edit(newRowNameIndex);
}

void ParametersTab::onRemoveAction()
{
    QModelIndex curIndex = _paramsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        models::db::RoutineParametersModel * model = _form->parametersModel();
        model->removeParamAt(curIndex.row());
        validateControls();
    }
}

void ParametersTab::onClearAction()
{
    models::db::RoutineParametersModel * model = _form->parametersModel();
    model->removeAllParams();
    validateControls();
}

void ParametersTab::onMoveUpAction()
{
    QModelIndex curIndex = _paramsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        models::db::RoutineParametersModel * model = _form->parametersModel();
        if (model->moveParamUp(curIndex.row())) {
            scrollToCurrent();
            validateControls();
        }
    }
}

void ParametersTab::onMoveDownAction()
{
    QModelIndex curIndex = _paramsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        models::db::RoutineParametersModel * model = _form->parametersModel();
        if (model->moveParamDown(curIndex.row())) {
            scrollToCurrent();
            validateControls();
        }
    }
}

void ParametersTab::currentRowChanged(const QModelIndex &current,
                                      const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    validateControls();
}

void ParametersTab::scrollToCurrent()
{
    _paramsTable->scrollTo(_paramsTable->selectionModel()->currentIndex());
}

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
