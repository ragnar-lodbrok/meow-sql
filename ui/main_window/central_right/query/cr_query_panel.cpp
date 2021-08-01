#include "cr_query_panel.h"
#include "central_right_query_tab.h"
#include "ui/common/sql_editor.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryPanel::QueryPanel(QueryTab * queryTab) 
    : QWidget(queryTab), 
      _queryTab(queryTab)
{
    createWidgets();
}

void QueryPanel::createWidgets()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(_mainLayout);

    createToolBar(); // put it temporarily here TODO: move to main win

    // TODO
    // http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
    // http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
    _queryTextEdit = new ui::common::SQLEditor();
    _mainLayout->addWidget(_queryTextEdit);

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void QueryPanel::createToolBar()
{
    _toolBar = new QToolBar();
    _toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
    #ifdef Q_OS_MAC
        _toolBar->setStyle(QStyleFactory::create("windows"));
    #endif

    _runAction = new QAction(QIcon(":/icons/execute.png"), tr("Run"), this);
    _runAction->setToolTip(tr("Execute SQL..."));
    _runAction->setShortcut(QKeySequence(Qt::Key_F9));
    connect(_runAction, &QAction::triggered, _queryTab, &QueryTab::onActionRun);
    _toolBar->addAction(_runAction);

    _mainLayout->addWidget(_toolBar, 0 , Qt::AlignTop);
}

QString QueryPanel::queryPlainText() const
{
    return _queryTextEdit->toPlainText();
}

void QueryPanel::setQueryText(const QString & text)
{
    _queryTextEdit->setPlainText(text);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
