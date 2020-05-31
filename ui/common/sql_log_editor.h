#ifndef UI_COMMON_SQL_LOG_EDITOR_H
#define UI_COMMON_SQL_LOG_EDITOR_H

#include "sql_editor.h"

namespace meow {
namespace ui {
namespace common {

class SQLLogEditor : public SQLEditor
{
public:
    explicit SQLLogEditor(QWidget * parent = nullptr);

    void appendMessage(const QString & message);

private:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

} // namespace common
} // namespace ui
} // namespace meow

#endif // UI_COMMON_SQL_LOG_EDITOR_H
