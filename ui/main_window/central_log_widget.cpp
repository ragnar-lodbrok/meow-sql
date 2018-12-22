#include "central_log_widget.h"
#include "ui/common/sql_log_editor.h"
#include "app/app.h"

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

    _logEditor = new ui::common::SQLLogEditor(this);
    _logEditor->setReadOnly(true);
    _mainLayout->addWidget(_logEditor);

    connect(meow::app()->actions()->logClear(),
            &QAction::triggered,
            this,
            &CentralLogWidget::onClearAction);
}

void CentralLogWidget::onLogMessage(const QString & msg)
{
    _logEditor->appendMessage(msg);
}

void CentralLogWidget::onClearAction(bool checked)
{
    Q_UNUSED(checked);
    _logEditor->clear();
}


} // namespace meow
} // namespace ui
} // namespace main_window
