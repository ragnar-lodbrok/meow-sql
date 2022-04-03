#ifndef UI_COMMON_SQL_EDITOR_H
#define UI_COMMON_SQL_EDITOR_H

// http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html


#include <QPlainTextEdit>
#include <QObject>
#include <QSyntaxHighlighter>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

namespace meow {
namespace ui {
namespace common {

class LineNumberArea;

enum class SyntaxHighligter {
    None,
    SQL
};

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget * parent = nullptr,
                       SyntaxHighligter syntax = SyntaxHighligter::None);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setHeightByRowCount(int rowCount);

protected:
    void resizeEvent(QResizeEvent * event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget * _lineNumberArea;
    QSyntaxHighlighter * _syntaxHighlighter;
};

class SQLEditor : public TextEditor
{
public:
    explicit SQLEditor(QWidget * parent = nullptr);

};

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(TextEditor * editor) : QWidget(editor), _codeEditor(editor)
    {

    }

    QSize sizeHint() const override
    {
        return QSize(_codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent * event) override
    {
        _codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEditor * _codeEditor;
};

} // namespace common
} // namespace ui
} // namespace meow

#endif // UI_COMMON_SQL_EDITOR_H
