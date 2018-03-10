#include "central_left_widget.h"
#include "central_left_db_tree.h"
#include <QDebug>

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

    _dbTree = new DbTree(this);
    _dbTree->setHeaderHidden(true);
    _dbTree->setModel(_dbEntitiesTreeModel);
    _mainLayout->addWidget(_dbTree);

    connect(_dbTree->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &CentralLeftWidget::selectedDbEntityChanged
    );
}

void CentralLeftWidget::selectEntity(meow::db::Entity * entity)
{

    QModelIndex curSelectionIndex = _dbTree->selectionModel()->currentIndex();
    QModelIndex newSelectionIndex = _dbEntitiesTreeModel->indexForEntity(entity);

    if (curSelectionIndex == newSelectionIndex) return;

    _dbTree->selectionModel()->setCurrentIndex(
                newSelectionIndex,
                QItemSelectionModel::ClearAndSelect);
    _dbTree->selectionModel()->select(
                newSelectionIndex,
                QItemSelectionModel::ClearAndSelect);
}

void CentralLeftWidget::selectedDbEntityChanged(
    const QItemSelection &selected,
    const QItemSelection &deselected)
{

    Q_UNUSED(deselected);

    QModelIndex index;
    QModelIndexList items = selected.indexes();

    if (!items.isEmpty()) {
        index = items.at(0);

        try {
            _dbEntitiesTreeModel->onSelectEntityAt(index);
        } catch(meow::db::Exception & ex) {
            qDebug() << "Tree error: " << ex.message();
            // TODO: show error
            // revert selection
        }

    } else {
        qDebug() << "Tree: selection is empty";
    }
}

} // namespace meow
} // namespace ui
} // namespace main_window
