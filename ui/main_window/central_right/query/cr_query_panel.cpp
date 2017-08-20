#include "cr_query_panel.h"
#include "db/user_query/sentences_parser.h" // test

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryPanel::QueryPanel(QWidget *parent) : QWidget(parent)
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
    _queryTextEdit = new QTextEdit();
    _mainLayout->addWidget(_queryTextEdit);
}

void QueryPanel::createToolBar()
{
    _toolBar = new QToolBar();
    _toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    _runAction = new QAction(QIcon(":/icons/execute.png"), tr("Run"), this);
    _runAction->setToolTip(tr("Execute SQL..."));
    _runAction->setShortcut(QKeySequence(Qt::Key_F9));
    connect(_runAction, &QAction::triggered, this, &QueryPanel::onActionRun);
    _toolBar->addAction(_runAction);

    _mainLayout->addWidget(_toolBar, 0 , Qt::AlignTop);
}

void QueryPanel::onActionRun(bool checked)
{
    Q_UNUSED(checked);
    meow::db::user_query::SentencesParser parser;
    qDebug() << "Run" << parser.parseByDelimiter(_queryTextEdit->toPlainText());
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
