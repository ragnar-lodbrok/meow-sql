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
    _queryTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    _mainLayout->addWidget(_queryTextEdit);

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    connect(_queryTextEdit,
            &QWidget::customContextMenuRequested,
            this,
            &QueryPanel::onQueryTextEditContextMenu);
}

void QueryPanel::createToolBar()
{
    _toolBar = new QToolBar();
    _toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
    #ifdef Q_OS_MAC
        _toolBar->setStyle(QStyleFactory::create("windows"));
    #endif

    _execQueryAction = new QAction(QIcon(":/icons/execute.png"),
                                   tr("Run"), this);
    _execQueryAction->setToolTip(tr("Execute SQL..."));
    _execQueryAction->setStatusTip(tr("Execute SQL-query/queries..."));
    _execQueryAction->setShortcut(QKeySequence(Qt::Key_F9));
    connect(_execQueryAction, &QAction::triggered,
            this, &QueryPanel::execQueryRequested);
    _toolBar->addAction(_execQueryAction);

    _execCurrentQueryAction = new QAction(QIcon(":/icons/execute_line.png"),
                                          tr("Run current query"), this);
    _execCurrentQueryAction->setToolTip(tr("Run current query"));
    _execCurrentQueryAction->setStatusTip(tr("Run currently focused SQL query"));
    _execCurrentQueryAction->setShortcut(QKeySequence(
                                             Qt::CTRL + Qt::SHIFT + Qt::Key_F9));
    connect(_execCurrentQueryAction, &QAction::triggered,
            this, &QueryPanel::onExecCurrentQueryAction);

    // TODO: add _execCurrentQueryAction to toolbar

    _separatorAction = new QAction(this);
    _separatorAction->setSeparator(true);

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

void QueryPanel::onQueryTextEditContextMenu(const QPoint & pos)
{
    // Listening: Disturbed - The Game

    QMenu * menu = _queryTextEdit->createStandardContextMenu();

    QList<QAction *> standardActions = menu->actions();

    QAction * firstStandardAction
            = standardActions.isEmpty() ? nullptr : standardActions.first();

    QList<QAction *> actions = {
        _execQueryAction,
        _execCurrentQueryAction
    };

    if (firstStandardAction) {
        actions << _separatorAction;
        menu->insertActions(firstStandardAction, actions);
    } else {
        menu->addActions(actions);
    }

    menu->exec(_queryTextEdit->mapToGlobal(pos));
    delete menu;
}

void QueryPanel::onExecCurrentQueryAction()
{
    int currentPosition = _queryTextEdit->textCursor().position();

    emit execCurrentQueryRequested(currentPosition);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
