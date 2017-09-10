#ifndef UI_COMMON_SQL_EDITOR_H
#define UI_COMMON_SQL_EDITOR_H

// http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html


#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

namespace meow {
namespace ui {
namespace common {

class LineNumberArea;
class SQLSyntaxHighlighter;

class SQLEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    SQLEditor(QWidget * parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent * event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget * _lineNumberArea;
    SQLSyntaxHighlighter * _syntaxHighlighter;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(SQLEditor * editor) : QWidget(editor), _codeEditor(editor)
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
    SQLEditor * _codeEditor;
};

} // namespace common
} // namespace ui
} // namespace meow

#endif // UI_COMMON_SQL_EDITOR_H
