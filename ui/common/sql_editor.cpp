#include "sql_editor.h"
#include <QtWidgets>
#include <QFontDatabase>
#include "sql_syntax_highlighter.h"

namespace meow {
namespace ui {
namespace common {

TextEditor::TextEditor(QWidget *parent, SyntaxHighligter syntax)
    : QPlainTextEdit(parent)
{

    // FixedFont does not work on MAC OS, it gives you a really tiny unreadable
    // Times-Roman
    #ifndef Q_OS_MAC
        QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        fixedFont.setStyleHint(QFont::Monospace);
        if (fixedFont.fixedPitch() == false) { // workaround for QTBUG-54623
            fixedFont.setFamily("monospace");
        }
        setFont(fixedFont);
    #endif
    _lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)),
            this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)),
            this, SLOT(updateLineNumberArea(QRect,int)));
    //connect(this, SIGNAL(cursorPositionChanged()),
    //        this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    //highlightCurrentLine();

    if (syntax == SyntaxHighligter::SQL) {
        _syntaxHighlighter = new SQLSyntaxHighlighter(this->document());
    }
}

int TextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 8 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void TextEditor::setHeightByRowCount(int rowCount)
{
    QFontMetrics fontMetrics(this->document()->defaultFont());
    QMargins margins = this->contentsMargins();

    int height = fontMetrics.lineSpacing() * rowCount;
    height += (this->frameWidth() + this->document()->documentMargin()) * 2;
    height += margins.top();
    height += margins.bottom();

    setFixedHeight(height);
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        _lineNumberArea->scroll(0, dy);
    else
        _lineNumberArea->update(0, rect.y(),
                                _lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    _lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                       lineNumberAreaWidth(), cr.height()));
}

void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor background = palette().color(QPalette::Window);
        background = background.toHsl();
        qreal newLightness = background.lightnessF();
        if (newLightness > 0.5) {
            newLightness -= 0.005;
        } else {
            newLightness += 0.005;
        }
        background.setHslF(background.hueF(),
                           background.saturationF(),
                           newLightness);
        QColor lineColor = background.toRgb();

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(_lineNumberArea);
    painter.fillRect(event->rect(), palette().color(QPalette::Window));

    QColor textColor = palette().color(QPalette::Text);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(textColor);
            painter.drawText(0, top,
                             _lineNumberArea->width() - 2,
                             fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

SQLEditor::SQLEditor(QWidget *parent)
    : TextEditor(parent, SyntaxHighligter::SQL)
{

}

} // namespace common
} // namespace ui
} // namespace meow
