#include "cr_table_info_indexes_tab.h"
#include "cr_table_indexes_tools.h"
#include "models/forms/table_info_form.h"
#include "models/forms/table_indexes_model.h"

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
}

void IndexesTab::refreshData()
{
    fillDataFromForm();
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
    _mainLayout->addWidget(_indexesTree, 1);
}

void IndexesTab::fillDataFromForm()
{

}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
