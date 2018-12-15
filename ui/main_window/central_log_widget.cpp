#include "central_log_widget.h"
#include "ui/common/sql_editor.h"

namespace meow {
namespace ui {
namespace main_window {

CentralLogWidget::CentralLogWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

void CentralLogWidget::createWidgets()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _logEditor = new ui::common::SQLEditor(this);
    _logEditor->setReadOnly(true);
    _mainLayout->addWidget(_logEditor);
}

void CentralLogWidget::onLogMessage(const QString & msg)
{
    appendMessage(msg);
}

void CentralLogWidget::appendMessage(const QString & message)
{
    bool isEmpty = _logEditor->document()->isEmpty();

    _logEditor->moveCursor(QTextCursor::End);
    if (!isEmpty) {
        _logEditor->insertPlainText(QString(QChar::LineFeed));
    }
    _logEditor->insertPlainText(message);
    _logEditor->moveCursor(QTextCursor::End);
}

} // namespace meow
} // namespace ui
} // namespace main_window
