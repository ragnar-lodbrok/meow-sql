#include "cr_table_columns_tools.h"
#include "cr_table_columns.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableColumnsTools::TableColumnsTools(TableColumns * parent)
    : QWidget(parent),
      _parent(parent)
{
    createWidgets();
}

void TableColumnsTools::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(2, 2, 0, 0);
    setLayout(mainLayout);

    _columnsLabel = new QLabel(tr("Columns:"));
    mainLayout->addWidget(_columnsLabel);

    _toolBar = new QToolBar();
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolBar->setMaximumHeight(32);
    mainLayout->addWidget(_toolBar);

    _addColumnAction = new QAction(QIcon(":/icons/add.png"), tr("Add"), this);
    _addColumnAction->setToolTip(tr("Add column"));
    connect(_addColumnAction, &QAction::triggered,
            _parent, &TableColumns::actionAddColumn);
    _toolBar->addAction(_addColumnAction);

    _removeColumnAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    _removeColumnAction->setToolTip(tr("Remove column"));
    connect(_removeColumnAction, &QAction::triggered,
            _parent, &TableColumns::actionRemoveColumn);
    _toolBar->addAction(_removeColumnAction);


    _moveUpColumnAction = new QAction(QIcon(":/icons/arrow_up.png"),
                                      tr("Up"), this);
    _moveUpColumnAction->setToolTip(tr("Move up"));
    connect(_moveUpColumnAction, &QAction::triggered,
            _parent, &TableColumns::actionMoveColumnUp);
    _toolBar->addAction(_moveUpColumnAction);


    _moveDownColumnAction = new QAction(QIcon(":/icons/arrow_down.png"),
                                      tr("Down"), this);
    _moveDownColumnAction->setToolTip(tr("Move down"));
    connect(_moveDownColumnAction, &QAction::triggered,
            _parent, &TableColumns::actionMoveColumnDown);
    _toolBar->addAction(_moveDownColumnAction);

    mainLayout->addStretch(1);

}

void TableColumnsTools::setActionEnabled(Action action, bool enabled)
{
    switch (action) {
    case Action::Add:
        _addColumnAction->setEnabled(enabled);
        break;
    case Action::Remove:
        _removeColumnAction->setEnabled(enabled);
        break;
    case Action::MoveUp:
        _moveUpColumnAction->setEnabled(enabled);
        break;
    case Action::MoveDown:
        _moveDownColumnAction->setEnabled(enabled);
        break;
    }
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
