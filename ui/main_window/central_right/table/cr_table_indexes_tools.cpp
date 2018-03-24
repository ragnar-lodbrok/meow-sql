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

    _addIndexAction = new QAction(QIcon(":/icons/add.png"),
                                  tr("Add index"), this);
    _addIndexAction->setToolTip(tr("Add index"));
    connect(_addIndexAction, &QAction::triggered,
            _parent, &IndexesTab::actionAddIndex);
    _toolBar->addAction(_addIndexAction);

    _addColumnAction = new QAction(QIcon(":/icons/bullet_white_add.png"),
                                   tr("Add column"), this);
    _addColumnAction->setToolTip(tr("Add column"));
    connect(_addColumnAction, &QAction::triggered,
            _parent, &IndexesTab::actionAddColumnToIndex);
    _toolBar->addAction(_addColumnAction);

    _removeAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    _removeAction->setToolTip(tr("Remove item"));
    connect(_removeAction, &QAction::triggered,
            _parent, &IndexesTab::actionRemoveCurrentIndexItem);
    _toolBar->addAction(_removeAction);

    _clearIndexesAction = new QAction(QIcon(":/icons/cross.png"),
                                      tr("Clear"), this);
    _clearIndexesAction->setToolTip(tr("Clear indexes"));
    connect(_clearIndexesAction, &QAction::triggered,
            _parent, &IndexesTab::actionClearIndexes);
    _toolBar->addAction(_clearIndexesAction);

    _moveUpColumnAction = new QAction(QIcon(":/icons/arrow_up.png"),
                                      tr("Up"), this);
    _moveUpColumnAction->setToolTip(tr("Move up"));
    connect(_moveUpColumnAction, &QAction::triggered,
            _parent, &IndexesTab::actionMoveUpColumn);
    _toolBar->addAction(_moveUpColumnAction);


    _moveDownColumnAction = new QAction(QIcon(":/icons/arrow_down.png"),
                                      tr("Down"), this);
    _moveDownColumnAction->setToolTip(tr("Move down"));
    connect(_moveDownColumnAction, &QAction::triggered,
            _parent, &IndexesTab::actionMoveDownColumn);
    _toolBar->addAction(_moveDownColumnAction);

    QLayout * toolBarLayout = _toolBar->layout();
    if (toolBarLayout) {
        toolBarLayout->setContentsMargins(0, 0, 0, 0);
        for (int i = 0; i < toolBarLayout->count(); ++i) {
            QLayoutItem * layoutItem = toolBarLayout->itemAt(i);
            layoutItem->setAlignment(Qt::AlignLeft);
            layoutItem->widget()->setMinimumWidth(110); // TODO: temp
        }
    }

    mainLayout->addStretch(1);
}

void TableIndexesTools::setActionEnabled(Action action, bool enabled)
{
    switch (action) {
    case Action::AddIndex:
        _addIndexAction->setEnabled(enabled);
        break;
    case Action::AddColumn:
        _addColumnAction->setEnabled(enabled);
        break;
    case Action::Remove:
        _removeAction->setEnabled(enabled);
        break;
    case Action::Clear:
        _clearIndexesAction->setEnabled(enabled);
        break;
    case Action::MoveUp:
        _moveUpColumnAction->setEnabled(enabled);
        break;
    case Action::MoveDown:
        _moveDownColumnAction->setEnabled(enabled);
        break;
    default:
        break;
    }
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
