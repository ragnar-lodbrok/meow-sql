#include "cr_table_indexes_tools.h"
#include "cr_table_info_indexes_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

TableIndexesTools::TableIndexesTools(IndexesTab * parent)
    : QWidget(parent),
      _parent(parent)
{
    createWidgets();
}

void TableIndexesTools::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    _toolBar = new QToolBar();
    _toolBar->setOrientation(Qt::Vertical);
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolBar->setIconSize(QSize(16, 16));
    mainLayout->addWidget(_toolBar);

    _addIndexAction = new QAction(QIcon(":/icons/add.png"), tr("Add"), this);
    _addIndexAction->setToolTip(tr("Add index"));
    _toolBar->addAction(_addIndexAction);

    _removeIndexAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    _removeIndexAction->setToolTip(tr("Remove index"));
    _toolBar->addAction(_removeIndexAction);

    _clearIndexesAction = new QAction(QIcon(":/icons/cross.png"),
                                      tr("Clear"), this);
    _clearIndexesAction->setToolTip(tr("Clear indexes"));
    _toolBar->addAction(_clearIndexesAction);

    _moveUpIndexAction = new QAction(QIcon(":/icons/arrow_up.png"),
                                      tr("Up"), this);
    _moveUpIndexAction->setToolTip(tr("Move up"));
    _toolBar->addAction(_moveUpIndexAction);


    _moveDownIndexAction = new QAction(QIcon(":/icons/arrow_down.png"),
                                      tr("Down"), this);
    _moveDownIndexAction->setToolTip(tr("Move down"));
    _toolBar->addAction(_moveDownIndexAction);

    QLayout * toolBarLayout = _toolBar->layout();
    if (toolBarLayout) {
        toolBarLayout->setContentsMargins(0, 0, 0, 0);
        for (int i = 0; i < toolBarLayout->count(); ++i) {
            QLayoutItem * layoutItem = toolBarLayout->itemAt(i);
            layoutItem->setAlignment(Qt::AlignLeft);
            layoutItem->widget()->setMinimumWidth(80); // TODO: temp
        }
    }

    mainLayout->addStretch(1);
}


} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
