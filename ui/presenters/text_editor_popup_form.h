#ifndef MODELS_FORMS_TEXT_EDITOR_POPUP_FORM_H
#define MODELS_FORMS_TEXT_EDITOR_POPUP_FORM_H

#include <QPlainTextEdit>
#include "helpers/text.h"

namespace meow {
namespace ui {
namespace presenters {

class TextEditorPopupForm : public QObject
{
    Q_OBJECT

public:
    TextEditorPopupForm();

    // in order to save memory don't store copy of text, use widget
    void setTextEdit(QPlainTextEdit * textEdit);

    void setText(const QString & text);

    void setWordWrap(bool wrap);

    void setLineBreaks(helpers::LineBreaks lineBreaks);

    void setMaxLength(int max) { _maxLength = max; }

    QString textWithCurLineBreaks() const;

    int lineNumber() const;

    int charCount() const {
        return _textEdit->document()->characterCount();
    }

    int maxLength() const {
        return _maxLength;
    }

    Q_SIGNAL void wordWrapChanged(bool wrap);
    Q_SIGNAL void lineBreaksChanged(helpers::LineBreaks lineBreaks,
                                    bool detected);

private:

    QPlainTextEdit * _textEdit;

    bool _wordWrap;
    helpers::LineBreaks _lineBreaks;
    int _maxLength;

};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MODELS_FORMS_TEXT_EDITOR_POPUP_FORM_H
