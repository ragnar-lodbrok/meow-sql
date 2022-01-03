#include "cr_table_info_foreign_keys_tab.h"
#include "cr_table_foreign_keys_tools.h"
#include "ui/presenters/table_info_form.h"
#include "ui/models/table_foreign_keys_model.h"
#include "ui/delegates/foreign_key_reference_option_delegate.h"
#include "ui/delegates/foreign_key_reference_table_delegate.h"
#include "ui/delegates/foreign_key_foreign_columns_delegate.h"
#include "ui/delegates/foreign_key_columns_delegate.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

ForeignKeysTab::ForeignKeysTab(presenters::TableInfoForm * form,
                               QWidget *parent)
    : QWidget(parent),
      _tableForm(form)
{
    Q_CHECK_PTR(form);
    createWidgets();
    validateControls();

    connect(form,
            &presenters::TableInfoForm::tableEngineChanged,
            [=] (const QString & engineName) {
                Q_UNUSED(engineName);
                validateControls();
            }
    );
}

void ForeignKeysTab::createWidgets()
{
    // TODO: show a message when db doesn't support FK

    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(_mainLayout);

    _tools = new TableForeignKeysTools(this);
    _mainLayout->addWidget(_tools, 0);

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

    _fKeysTable->setSelectionBehavior(
          QAbstractItemView::SelectionBehavior::SelectRows);
    _fKeysTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(_fKeysTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &ForeignKeysTab::currentKeyChanged
    );

    _mainLayout->addWidget(_fKeysTable, 1);

    // delegates ---------------------------------------------------------------

    delegates::ForeignKeyReferenceOptionDelegate * refOptionsDelegate
        = new delegates::ForeignKeyReferenceOptionDelegate(model);
    _fKeysTable->setItemDelegateForColumn(
        (int)models::TableForeignKeysModel::Columns::OnUpdate,
        refOptionsDelegate);
    _fKeysTable->setItemDelegateForColumn(
        (int)models::TableForeignKeysModel::Columns::OnDelete,
        refOptionsDelegate);

    delegates::ForeignKeyReferenceTableDelegate * refTableDelegate
        = new delegates::ForeignKeyReferenceTableDelegate(model);
    _fKeysTable->setItemDelegateForColumn(
        (int)models::TableForeignKeysModel::Columns::ReferenceTable,
        refTableDelegate);

    delegates::ForeignKeyForeignColumnsDelegate * frgnColumnsDelegate
        = new delegates::ForeignKeyForeignColumnsDelegate(model);
    _fKeysTable->setItemDelegateForColumn(
        (int)models::TableForeignKeysModel::Columns::ForeignColumns,
        frgnColumnsDelegate);

    delegates::ForeignKeyColumnsDelegate * columnsDelegate
        = new delegates::ForeignKeyColumnsDelegate(model);
    _fKeysTable->setItemDelegateForColumn(
        (int)models::TableForeignKeysModel::Columns::Columns,
        columnsDelegate);

}

void ForeignKeysTab::refreshData()
{
    validateControls();
}

void ForeignKeysTab::validateControls()
{
    models::TableForeignKeysModel * model
        = _tableForm->foreignKeysModel();

    using ColAction = TableForeignKeysTools::Action;

    QModelIndex curIndex = _fKeysTable->selectionModel()->currentIndex();

    _tools->setActionEnabled(ColAction::Add,    model->canAddKey());
    _tools->setActionEnabled(ColAction::Clear,  model->canRemoveAllKeys());
    _tools->setActionEnabled(ColAction::Remove, model->canRemoveKey(curIndex));

}

void ForeignKeysTab::actionAddKey(bool checked)
{
    Q_UNUSED(checked);

    auto model = _tableForm->foreignKeysModel();

    // insert
    int newRowIntIndex = model->insertEmptyDefaultKey();

    // select
    QModelIndex newRowNameIndex = model->index(
        newRowIntIndex,
        static_cast<int>(models::TableForeignKeysModel::Columns::Name));
    _fKeysTable->selectionModel()->setCurrentIndex(
        newRowNameIndex,
        QItemSelectionModel::ClearAndSelect);

    // scroll
    _fKeysTable->scrollTo(_fKeysTable->selectionModel()->currentIndex());

    validateControls();
}

void ForeignKeysTab::actionRemoveKey(bool checked)
{
    Q_UNUSED(checked);

    QModelIndex curIndex = _fKeysTable->selectionModel()->currentIndex();
    if (!curIndex.isValid()) return;

    auto model = _tableForm->foreignKeysModel();
    model->removeKey(curIndex);
    validateControls();
}

void ForeignKeysTab::actionClearKeys(bool checked)
{
    Q_UNUSED(checked);
    _tableForm->foreignKeysModel()->removeAllKeys();
    validateControls();
}

void ForeignKeysTab::currentKeyChanged(
        const QModelIndex &current,
        const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    validateControls();
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
