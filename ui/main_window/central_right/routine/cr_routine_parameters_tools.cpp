#include "cr_routine_parameters_tools.h"
#include "cr_routine_info_parameters_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace routine_info {


RoutineParametersTools::RoutineParametersTools(ParametersTab * parent)
    : QWidget(parent)
    , _parent(parent)
{
    createWidgets();
}

void RoutineParametersTools::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    _toolBar = new QToolBar();
    _toolBar->setOrientation(Qt::Vertical);
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolBar->setIconSize(QSize(16, 16));
    #ifdef Q_OS_MAC
        _toolBar->setStyle(QStyleFactory::create("windows"));
    #endif
    mainLayout->addWidget(_toolBar);

    _addAction = new QAction(QIcon(":/icons/add.png"),
                                  tr("Add"), this);
    connect(_addAction, &QAction::triggered,
            _parent, &ParametersTab::onAddAction);
    _toolBar->addAction(_addAction);

    _removeAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    connect(_removeAction, &QAction::triggered,
            _parent, &ParametersTab::onRemoveAction);
    _toolBar->addAction(_removeAction);

    _clearAction = new QAction(QIcon(":/icons/cross.png"),
                                      tr("Clear"), this);
    connect(_clearAction, &QAction::triggered,
            _parent, &ParametersTab::onClearAction);
    _toolBar->addAction(_clearAction);

    _moveUpAction = new QAction(QIcon(":/icons/arrow_up.png"),
                                      tr("Up"), this);
    _moveUpAction->setToolTip(tr("Move up"));
    connect(_moveUpAction, &QAction::triggered,
            _parent, &ParametersTab::onMoveUpAction);
    _toolBar->addAction(_moveUpAction);


    _moveDownAction = new QAction(QIcon(":/icons/arrow_down.png"),
                                      tr("Down"), this);
    _moveDownAction->setToolTip(tr("Move down"));
    connect(_moveDownAction, &QAction::triggered,
            _parent, &ParametersTab::onMoveDownAction);
    _toolBar->addAction(_moveDownAction);

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

void RoutineParametersTools::setActionEnabled(Action action, bool enabled)
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
    case Action::MoveUp:
        _moveUpAction->setEnabled(enabled);
        break;
    case Action::MoveDown:
        _moveDownAction->setEnabled(enabled);
        break;
    }
}

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
