#include "central_left_widget.h"

namespace meow {
namespace ui {
namespace main_window {

CentralLeftWidget::CentralLeftWidget(models::db::EntitiesTreeModel * dbEntitiesTreeModel,
        QWidget * parent)
    :QWidget(parent),
     _dbEntitiesTreeModel(dbEntitiesTreeModel)
{
    createMainLayout();
}

void CentralLeftWidget::createMainLayout()
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _dbTree = new QTreeView();
    _dbTree->setHeaderHidden(true);
    _dbTree->setModel(_dbEntitiesTreeModel);
    _mainLayout->addWidget(_dbTree);
}

} // namespace meow
} // namespace ui
} // namespace main_window
