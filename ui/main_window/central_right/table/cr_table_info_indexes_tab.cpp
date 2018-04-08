#include "cr_table_info_indexes_tab.h"
#include "cr_table_indexes_tools.h"
#include "models/forms/table_info_form.h"
#include "models/forms/table_indexes_model.h"
#include "models/delegates/table_index_delegate.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

IndexesTab::IndexesTab(models::forms::TableInfoForm * form,
                       QWidget *parent)
    : QWidget(parent),
      _tableForm(form)
{
    Q_CHECK_PTR(form);
    createWidgets();
    validateControls();

    connect(form,
            &models::forms::TableInfoForm::tableColumnRemoved,
            this,
            &IndexesTab::onTableColumnRemoved
    );
}

void IndexesTab::refreshData()
{
    fillDataFromForm();
    validateControls();
}

void IndexesTab::createWidgets()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(_mainLayout);

    _tools = new TableIndexesTools(this);
    _mainLayout->addWidget(_tools, 0);

    _indexesTree = new QTreeView();
    auto treeModel = _tableForm->indexesModel();
    _indexesTree->setModel(treeModel);
    for (int i = 0; i < treeModel->columnCount(); ++i) {
        _indexesTree->setColumnWidth(i, treeModel->columnWidth(i));
    }
    connect(_indexesTree->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &IndexesTab::selectedIndexItemChanged
    );
    auto treeDelegate = new models::delegates::TableIndexDelegate(treeModel);
    _indexesTree->setItemDelegate(treeDelegate);
    _mainLayout->addWidget(_indexesTree, 1);
}

void IndexesTab::selectedIndexItemChanged(
    const QItemSelection &selected,
    const QItemSelection &deselected)
{

    Q_UNUSED(deselected);
    Q_UNUSED(selected);

    validateControls();
}

void IndexesTab::fillDataFromForm()
{

}

void IndexesTab::validateControls()
{
    auto model = _tableForm->indexesModel();

    using ColAction = TableIndexesTools::Action;

    QModelIndex curIndex = _indexesTree->selectionModel()->currentIndex();

    _tools->setActionEnabled(ColAction::AddIndex,  model->canAddIndex());
    _tools->setActionEnabled(ColAction::Clear,     model->canRemoveAll());

    if (curIndex.isValid()) {
        _tools->setActionEnabled(ColAction::AddColumn, model->canAddColumn(curIndex));
        _tools->setActionEnabled(ColAction::Remove,    model->canRemove(curIndex));
        _tools->setActionEnabled(ColAction::MoveUp,    model->canMoveUp(curIndex));
        _tools->setActionEnabled(ColAction::MoveDown,  model->canMoveDown(curIndex));
    } else {
        _tools->setActionEnabled(ColAction::AddColumn, false);
        _tools->setActionEnabled(ColAction::Remove,    false);
        _tools->setActionEnabled(ColAction::MoveUp,    false);
        _tools->setActionEnabled(ColAction::MoveDown,  false);
    }
}

void IndexesTab::actionAddIndex(bool checked)
{
    Q_UNUSED(checked);

    auto model = _tableForm->indexesModel();

    // insert
    int newRowIntIndex = model->insertEmptyDefaultIndex();

    // select
    QModelIndex newRowNameIndex = model->index(
        newRowIntIndex,
        static_cast<int>(meow::models::forms::TableIndexesModel::Columns::Name));
    _indexesTree->selectionModel()->setCurrentIndex(
        newRowNameIndex,
        QItemSelectionModel::ClearAndSelect);

    // scroll
    _indexesTree->scrollTo(_indexesTree->selectionModel()->currentIndex());

    validateControls();
}

void IndexesTab::actionAddColumnToIndex(bool checked)
{
    Q_UNUSED(checked);

    QModelIndex curIndex = _indexesTree->selectionModel()->currentIndex();
    if (!curIndex.isValid()) return;

    auto model = _tableForm->indexesModel();

    QModelIndex insertedIndex = model->insertEmptyColumn(curIndex);

    if (insertedIndex.isValid()) {
        _indexesTree->selectionModel()->setCurrentIndex(
            insertedIndex,
            QItemSelectionModel::ClearAndSelect);
        _indexesTree->scrollTo(_indexesTree->selectionModel()->currentIndex());
    }

    validateControls();
}

void IndexesTab::actionRemoveCurrentIndexItem(bool checked)
{
    Q_UNUSED(checked);

    QModelIndex curIndex = _indexesTree->selectionModel()->currentIndex();
    if (!curIndex.isValid()) return;

    auto model = _tableForm->indexesModel();
    model->remove(curIndex);
    validateControls();

}

void IndexesTab::actionClearIndexes(bool checked)
{
    Q_UNUSED(checked);
    _tableForm->indexesModel()->removeAll();
    validateControls();
}

void IndexesTab::actionMoveUpColumn(bool checked)
{
    Q_UNUSED(checked);
}

void IndexesTab::actionMoveDownColumn(bool checked)
{
    Q_UNUSED(checked);
}

void IndexesTab::onTableColumnRemoved(const QString & name)
{
    auto model = _tableForm->indexesModel();
    model->removeAllIndexColumnsByName(name);
    validateControls();
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
