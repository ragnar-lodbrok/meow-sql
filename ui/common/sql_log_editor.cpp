#include "sql_log_editor.h"
#include <QMenu>
#include "app/app.h"

namespace meow {
namespace ui {
namespace common {

SQLLogEditor::SQLLogEditor(QWidget * parent) : SQLEditor(parent)
{

}

void SQLLogEditor::appendMessage(const QString & message)
{
    bool isEmpty = this->document()->isEmpty();

    moveCursor(QTextCursor::End);
    if (!isEmpty) {
        insertPlainText(QString(QChar::LineFeed));
    }
    insertPlainText(message);
    moveCursor(QTextCursor::End);
}

void SQLLogEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu * menu = createStandardContextMenu();
    menu->addAction(meow::app()->actions()->logClear());

    menu->exec(event->globalPos());
    delete menu;
}

} // namespace common
} // namespace ui
} // namespace meow
