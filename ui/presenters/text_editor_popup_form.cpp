#include "text_editor_popup_form.h"
#include "app/app.h"

static const meow::helpers::LineBreaks TEXT_EDIT_LINE_BREAKS =
        meow::helpers::LineBreaks::Unix; // QPlainTextEdit always has \n

namespace meow {
namespace ui {
namespace presenters {

TextEditorPopupForm::TextEditorPopupForm()
    : _textEdit(nullptr)
    , _wordWrap(false) // read TODO: from settings
    , _lineBreaks(helpers::LineBreaks::None)
    , _maxLength(0)
{

}

void TextEditorPopupForm::setTextEdit(QPlainTextEdit * textEdit)
{
    Q_CHECK_PTR(textEdit);
    _textEdit = textEdit;

    emit wordWrapChanged(_wordWrap);
}

void TextEditorPopupForm::setText(const QString & text)
{
    bool detected = true;
    _lineBreaks = helpers::scanLineBreaks(text);
    if (_lineBreaks == helpers::LineBreaks::None) {
        _lineBreaks = meow::app()->settings()
                            ->textSettings()->defaultLineBreaks();
        detected = false;
    }

    emit lineBreaksChanged(_lineBreaks, detected);
}

void TextEditorPopupForm::setWordWrap(bool wrap)
{
    bool changed = _wordWrap != wrap;
    _wordWrap = wrap;
    if (changed) {
        emit wordWrapChanged(wrap);
    }
}

void TextEditorPopupForm::setLineBreaks(helpers::LineBreaks lineBreaks)
{
    if (_lineBreaks != lineBreaks) {
        _lineBreaks = lineBreaks;
        emit lineBreaksChanged(_lineBreaks, false);
    }
}

QString TextEditorPopupForm::textWithCurLineBreaks() const
{
    QString text = _textEdit->toPlainText();

    return helpers::replaceLineBreaks(
            text,
            TEXT_EDIT_LINE_BREAKS,
            _lineBreaks
    );
}

int TextEditorPopupForm::lineNumber() const
{
    // _textEdit->document()->lineCount() is wrong when wordwrap

    return helpers::countLineBreaks(_textEdit->toPlainText(),
                                    TEXT_EDIT_LINE_BREAKS) + 1;
}

} // namespace presenters
} // namespace ui
} // namespace meow
