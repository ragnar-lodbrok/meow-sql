#include "sql_syntax_highlighter.h"
#include "mysql_syntax.h"
#include "db/user_query/sentences_parser.h"
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>

namespace meow {
namespace ui {
namespace common {

SQLSyntaxHighlighter::SQLSyntaxHighlighter(QTextDocument * parent)
    :QSyntaxHighlighter(parent)
{

    QColor textColor = QGuiApplication::palette().color(QPalette::WindowText);
    bool isLightTheme
        = (textColor.redF() + textColor.greenF() + textColor.blueF()) < 0.5*3;


    _singleLineCommentFormat.setForeground(QColor(149, 149, 158));
    _multiLineCommentFormat = _singleLineCommentFormat;
    _quotationFormat.setForeground(
        isLightTheme ? QColor(102, 153, 0) : QColor(115, 213, 115));
    _reservedKeywordFormat.setForeground(
        isLightTheme ? QColor(0, 119, 170) : QColor(80, 239, 239));
    _boolLiteralsFormat.setForeground(
        isLightTheme ? QColor(153, 0, 85) : QColor(201, 115, 115));
    _numericFormat.setForeground(
        isLightTheme ? QColor(153, 0, 85) : QColor(201, 115, 115));
    _functionFormat.setForeground(QColor(221, 74, 104));

    addKeywordsRules();
}

void SQLSyntaxHighlighter::addKeywordsRules()
{

    QStringList reservedKeywords = meow::db::common::mySqlReservedKeywords();
    QStringList boolLiterals = meow::db::common::mySqlBoolLiterals();

    for (const QString &word : boolLiterals) {
        reservedKeywords.removeOne(word);
    }

    appendKeywords(reservedKeywords, _reservedKeywordFormat);
    appendKeywords(boolLiterals,     _boolLiteralsFormat);

    //TODO: this is stupid but better than nothing
    QString floatPattern(R"([-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)");
    HighlightingRule rule;
    rule.pattern = QRegularExpression(QString(R"(\b%1\b)").arg(floatPattern));
    rule.format = _numericFormat;

    _keywordsRules.append(rule);
}

void SQLSyntaxHighlighter::appendKeywords(
    const QStringList & words,
    const QTextCharFormat & format)
{
    HighlightingRule rule;

    for (const QString &keyword : words) {
        QString pattern = QString(R"(\b%1\b)").arg(keyword);
        rule.pattern = QRegularExpression
                (pattern, QRegularExpression::CaseInsensitiveOption);
        rule.format = format;
        _keywordsRules.append(rule);
    }
}

void SQLSyntaxHighlighter::highlightBlock(const QString &text)
{

    namespace uq = meow::db::user_query;

    uq::SentenceTokenType prevOpenType = uq::SentenceTokenType::None;

    int prevBlockState = previousBlockState();
    if (prevBlockState != -1) {
        prevOpenType = static_cast<uq::SentenceTokenType>(prevBlockState);
    }

    setCurrentBlockState(-1);

    uq::SentencesParser parser;
    auto tokens = parser.parseToTokens(text, prevOpenType);

    for (const auto & token : tokens) {

        switch (token->type) {

        case uq::SentenceTokenType::Text: {

            QString tokenText = text.mid(token->startIndex, token->len);

            for (const HighlightingRule &rule : _keywordsRules) {
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

        case uq::SentenceTokenType::SingleLineComment:
            setFormat(
                        token->startIndex,
                        token->len,
                        _singleLineCommentFormat);
            break;

        case uq::SentenceTokenType::MultipleLineComment:
            setFormat(
                        token->startIndex,
                        token->len,
                        _multiLineCommentFormat);
            break;

        case uq::SentenceTokenType::QuotedString:
        case uq::SentenceTokenType::DoubleQuotedString:
            setFormat(
                        token->startIndex,
                        token->len,
                        _quotationFormat);
            break;

        default:
            break;
        }
    }

    if (!tokens.isEmpty()) {
        auto lastToken = tokens.last();
        if (lastToken->rightOpen) {
            setCurrentBlockState(static_cast<int>(lastToken->type));
        }
    } else { // empty text
        setCurrentBlockState(previousBlockState());
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
