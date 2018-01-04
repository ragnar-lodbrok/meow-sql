#include "cr_table_columns_tools.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableColumnsTools::TableColumnsTools(QWidget * parent) : QWidget(parent)
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
    //connect(_addColumnAction, &QAction::triggered, this,
    //       &TableColumnsTools::actionAddColumn);
    _toolBar->addAction(_addColumnAction);

    _removeColumnAction = new QAction(QIcon(":/icons/delete.png"),
                                      tr("Remove"), this);
    _removeColumnAction->setToolTip(tr("Remove column"));
    _toolBar->addAction(_removeColumnAction);


    _moveUpColumnAction = new QAction(QIcon(":/icons/arrow_up.png"),
                                      tr("Up"), this);
    _moveUpColumnAction->setToolTip(tr("Move up"));
    _toolBar->addAction(_moveUpColumnAction);


    _moveDownColumnAction = new QAction(QIcon(":/icons/arrow_down.png"),
                                      tr("Down"), this);
    _moveDownColumnAction->setToolTip(tr("Move down"));
    _toolBar->addAction(_moveDownColumnAction);

    mainLayout->addStretch(1);

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
