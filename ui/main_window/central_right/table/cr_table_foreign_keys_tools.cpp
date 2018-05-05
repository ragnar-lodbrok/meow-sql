#include "cr_table_foreign_keys_tools.h"
#include "cr_table_info_foreign_keys_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

TableForeignKeysTools::TableForeignKeysTools(ForeignKeysTab * parent)
    : QWidget(parent),
      _parent(parent)
{
    createWidgets();
}

void TableForeignKeysTools::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    _toolBar = new QToolBar();
    _toolBar->setOrientation(Qt::Vertical);
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolBar->setIconSize(QSize(16, 16));
    mainLayout->addWidget(_toolBar);

    _addAction = new QAction(QIcon(":/icons/add.png"),
                                  tr("Add"), this);
    connect(_addAction, &QAction::triggered,
            _parent, &ForeignKeysTab::actionAddKey);
    _toolBar->addAction(_addAction);

    _removeAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    connect(_removeAction, &QAction::triggered,
            _parent, &ForeignKeysTab::actionRemoveKey);
    _toolBar->addAction(_removeAction);

    _clearAction = new QAction(QIcon(":/icons/cross.png"),
                                      tr("Clear"), this);
    connect(_clearAction, &QAction::triggered,
            _parent, &ForeignKeysTab::actionClearKeys);
    _toolBar->addAction(_clearAction);


    QLayout * toolBarLayout = _toolBar->layout();
    if (toolBarLayout) {
        toolBarLayout->setContentsMargins(0, 0, 0, 0);
        for (int i = 0; i < toolBarLayout->count(); ++i) {
            QLayoutItem * layoutItem = toolBarLayout->itemAt(i);
            layoutItem->setAlignment(Qt::AlignLeft);
            layoutItem->widget()->setMinimumWidth(85); // TODO: temp
        }
    }

    mainLayout->addStretch(1);
}

void TableForeignKeysTools::setActionEnabled(Action action, bool enabled)
{
    switch (action) {
    case Action::Add:
        _addAction->setEnabled(enabled);
        break;
    case Action::Remove:
        _removeAction->setEnabled(enabled);
        break;
    case Action::Clear:
        _clearAction->setEnabled(enabled);
        break;
    }
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
