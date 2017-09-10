#include "sql_syntax_highlighter.h"
#include "mysql_syntax.h"


namespace meow {
namespace ui {
namespace common {

SQLSyntaxHighlighter::SQLSyntaxHighlighter(QTextDocument * parent)
    :QSyntaxHighlighter(parent)
{
    addReservedKeywordsRules();

    _singleLineCommentFormat.setForeground(QColor(149, 149, 158));
}

void SQLSyntaxHighlighter::addReservedKeywordsRules()
{
    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(0, 119, 170));

    QStringList keywords = meow::db::common::mySqlReservedKeywords();

    for (const QString &keyword : keywords) {
        QString pattern = QString(R"(\b%1\b)").arg(keyword);
        rule.pattern = QRegularExpression
                (pattern, QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        _reservedKeywordsRules.append(rule);
    }
}

void SQLSyntaxHighlighter::highlightBlock(const QString &text)
{
    int signleLineCommentStart = findSingleLineCommentStart(text);

    QString textWithoutComment = text;

    if (signleLineCommentStart > 0) {
        textWithoutComment = text.left(signleLineCommentStart);
    }

    for (const HighlightingRule &rule : _reservedKeywordsRules) {
        QRegularExpressionMatchIterator matchIterator =
                rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            if (isQuotedId(text, match)) {
                continue;
            }

            setFormat(
                        match.capturedStart(),
                        match.capturedLength(),
                        rule.format);
        }
    }

    if (signleLineCommentStart >= 0) {
        setFormat(
                    signleLineCommentStart,
                    text.length() - signleLineCommentStart,
                    _singleLineCommentFormat);
    }
}

bool SQLSyntaxHighlighter::isQuotedId(
        const QString & text,
        const QRegularExpressionMatch & match) const
{
    int startIndex = match.capturedStart();
    int endIndex = match.capturedEnd();

    if (startIndex > 0) {
        if (text.at(startIndex-1) == QChar('.')) {
            return true;
        } else if (text.at(startIndex-1) == QChar('`')) {
            if (endIndex < text.length()) {
                return text.at(endIndex) == QChar('`');
            }
        }
    }

    return false;
}

int SQLSyntaxHighlighter::findSingleLineCommentStart(const QString &text)
{
    int len = text.length();

    bool inEscape = false;
    bool inString = false;
    QChar lastStringEncloser = QChar::Null;

    for (int i=0; i<len; ++i) {
        QChar curChar = text.at(i);

        if (!inEscape) {
            if (curChar == QChar('"') ||
                curChar == QChar('\'') ||
                curChar == QChar('`')) { // str or id enclosers
                if (!inString || (inString && curChar == lastStringEncloser)) {
                    inString = !inString;
                    lastStringEncloser = curChar;
                }
            }
        }

        if (!inString) {
            QChar nextChar = ((i+1) < len) ? text.at(i+1) : QChar::Null;
            bool inLineComment = (curChar == QChar('#'))
                || (curChar == QChar('-') && nextChar == QChar('-'));
            if (inLineComment) {
                return i;
            }
        }

        if (!inEscape) {
            inEscape = (curChar == QChar('\\'));
        } else {
            inEscape = false;
        }
    }


    return -1;
}

} // namespace common
} // namespace ui
} // namespace meow
