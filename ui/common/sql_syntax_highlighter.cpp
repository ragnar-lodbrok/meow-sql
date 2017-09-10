#include "sql_syntax_highlighter.h"
#include "mysql_syntax.h"
#include "db/user_query/sentences_parser.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace common {

SQLSyntaxHighlighter::SQLSyntaxHighlighter(QTextDocument * parent)
    :QSyntaxHighlighter(parent)
{
    addReservedKeywordsRules();

    _singleLineCommentFormat.setForeground(QColor(149, 149, 158));
    _multiLineCommentFormat = _singleLineCommentFormat;
    _quotationFormat.setForeground(QColor(102, 153, 0));
    _reservedKeywordFormat.setForeground(QColor(0, 119, 170));
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

    meow::db::user_query::SentencesParser parser;
    auto tokens = parser.parseToTokens(text);

    for (const auto & token : tokens) {

        switch (token->type) {

        case meow::db::user_query::SentenceTokenType::Text: {

            QString tokenText = text.mid(token->startIndex, token->len);

            for (const HighlightingRule &rule : _reservedKeywordsRules) {
                QRegularExpressionMatchIterator matchIterator =
                        rule.pattern.globalMatch(tokenText);

                while (matchIterator.hasNext()) {

                    QRegularExpressionMatch match = matchIterator.next();

                    //if (isQuotedId(tokenText, match)) {
                    //    continue;
                    //}

                    setFormat(
                                match.capturedStart() + token->startIndex,
                                match.capturedLength(),
                                rule.format);
                }
            }
            break;
        }

        case meow::db::user_query::SentenceTokenType::SingleLineComment:
            setFormat(
                        token->startIndex,
                        token->len,
                        _singleLineCommentFormat);
            break;

        case meow::db::user_query::SentenceTokenType::MultipleLineComment:
            setFormat(
                        token->startIndex,
                        token->len,
                        _multiLineCommentFormat);
            break;

        case meow::db::user_query::SentenceTokenType::QuotedString:
            setFormat(
                        token->startIndex,
                        token->len,
                        _quotationFormat);
            break;

        default:
            break;
        }
    }
}

bool SQLSyntaxHighlighter::isQuotedId( // rm
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

} // namespace common
} // namespace ui
} // namespace meow
