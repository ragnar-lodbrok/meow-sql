#ifndef UI_COMMON_SQL_SYNTAX_HIGHLIGHTER_H
#define UI_COMMON_SQL_SYNTAX_HIGHLIGHTER_H

// http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

namespace meow {
namespace ui {
namespace common {

class SQLSyntaxHighlighter : public QSyntaxHighlighter
{

public:
    SQLSyntaxHighlighter(QTextDocument * parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:

    void addReservedKeywordsRules();
    void createSingleLineCommentRules();

    bool isQuotedId(const QString &text,
                    const QRegularExpressionMatch & match) const;

    int findSingleLineCommentStart(const QString &text);

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> _reservedKeywordsRules;

    QTextCharFormat _singleLineCommentFormat;

    //QRegularExpression commentStartExpression;
    //QRegularExpression commentEndExpression;

    //QTextCharFormat keywordFormat;
    //QTextCharFormat singleLineCommentFormat;
    //QTextCharFormat multiLineCommentFormat;
    //QTextCharFormat quotationFormat;
    //QTextCharFormat functionFormat;
};

} // namespace common
} // namespace ui
} // namespace meow

#endif // UI_COMMON_SQL_SYNTAX_HIGHLIGHTER_H
