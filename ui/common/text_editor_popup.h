#ifndef UI_TEXT_EDITOR_POPUP_H
#define UI_TEXT_EDITOR_POPUP_H

#include <QtWidgets>
#include "helpers/text.h"

namespace meow {
namespace ui {


class TextEditorPopup : public QDialog
{
public:
    TextEditorPopup();

private:
    void createWidgets();
    void createActions();

    QAction * defaultLineBreakAction() const;
    void updateLineBreaksButtonInfo(QAction * action);

    Q_SLOT void onLineBreaksAction();

    QPlainTextEdit * _textEdit;
    QLabel * _textStat;
    QStatusBar * _statusBar;
    QToolBar * _toolbar;
    QToolButton * _lineBreaksButton;

    QAction * _wordWrapAction;
    QMap<helpers::LineBreaks, QAction *> _lineBreaksActions;
    QAction * _cancelAction;
    QAction * _applyAction;
};

} // namespace ui
} // namespace meow

#endif // UI_TEXT_EDITOR_POPUP_H
