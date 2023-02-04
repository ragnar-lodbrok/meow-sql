#ifndef UI_TEXT_EDITOR_POPUP_H
#define UI_TEXT_EDITOR_POPUP_H

#include <QtWidgets>
#include "helpers/text.h"
#include "ui/presenters/text_editor_popup_form.h"

#include "ui/common/sql_editor.h"

namespace meow {
namespace ui {


class TextEditorPopup : public QDialog
{
    Q_OBJECT
public:
    TextEditorPopup();

    void setText(const QString & text);
    void setTitleText(const QString & text);

    QString text() const;

private:
    void createWidgets();
    void createActions();

    QAction * defaultLineBreakAction() const;
    void updateLineBreaksButtonInfo(QAction * action);

    QString textStats() const;

    Q_SLOT void onLineBreaksAction();
    Q_SLOT void onWordWrapToggled(bool checked);
    Q_SLOT void onTextChanged();

    presenters::TextEditorPopupForm _form;

    ui::common::TextEditor * _textEdit;
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
