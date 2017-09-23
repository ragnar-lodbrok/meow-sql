#include "sentences_parser.h"
#include <QDebug>

namespace meow {
namespace db {
namespace user_query {

QStringList SentencesParser::parseByDelimiter(const QString &SQL,
                                              const QString &delim) const
{
    // Let's make Chomsky cry

    QStringList list;

    // to unix line ends
    QString str = SQL;
    str.replace("\r\n", "\n", Qt::CaseInsensitive); // win
    str.replace(QChar('\r'), QChar('\n'), Qt::CaseInsensitive); // mac
    int allLen = str.length();
    QChar curChar;
    QChar nextChar;
    QString curDelim = delim; // may be changed by SQL

    bool inString = false;// in "enclosed string" or `identifier`
    bool inLineComment = false; // in one-line comment (# or --)
    bool inBigComment = false; // in /* multi-line */ or /*! conditional comment */
    bool inEscape = false; // Previous char was backslash
    QChar lastStringEncloser = QChar::Null;
    int lastLeftOffset = 0;

    for (int i=0; i<allLen; ++i) {
        curChar = str.at(i);
        nextChar = ((i+1) < allLen) ? str.at(i+1) : QChar::Null;

        // 1. Check for comment syntax and for enclosed literals,
        // so a query delimiter can be ignored

        // inLineComment
        if (!inLineComment && !inBigComment && !inString) {
            inLineComment = (curChar == QChar('#'))
                || (curChar == QChar('-') && nextChar == QChar('-'));
            // TODO "--" requires space after?
        }

        // inBigComment
        if (!inLineComment && !inBigComment && !inString) {
            if (curChar == QChar('/') && nextChar == QChar('*')) {
                inBigComment = true;
            }
        }
        if (inBigComment && !inLineComment && !inString) {
            if (curChar == QChar('*') && nextChar == QChar('/')) {
                inBigComment = false;
            }
        }

        // inString
        if (!inEscape && !inLineComment && !inBigComment) {
            if (curChar == QChar('"') ||
                curChar == QChar('\'') ||
                curChar == QChar('`')) { // str or id enclosers
                if (!inString || (inString && curChar == lastStringEncloser)) {
                    inString = !inString;
                    lastStringEncloser = curChar;
                }
            }
        }

        // new lines
        if ((curChar == QChar::LineFeed && nextChar != QChar::LineFeed) || i==0) {
            if (i > 0) {
                inLineComment = false;
            }
            // TODO: detect DELIMITER query
        }

        // inEscape
        if (!inEscape) {
            inEscape = (curChar == QChar('\\'));
        } else {
            inEscape = false;
        }

        // Delim
        bool onDelim = false;
        if (!inLineComment && !inString && !inBigComment) {
            int delimStart = std::max(0, i+1-curDelim.length()); // TODO: ?
            QStringRef curDelimRef = str.midRef(delimStart, curDelim.length());
            onDelim = curDelimRef == curDelim;
        }

        if (onDelim || (i+1 == allLen)) {
            int rightOffset = i+1;
            if (onDelim) {
                rightOffset -= curDelim.length();
            }
            QStringRef queryTest =
                    str.midRef(lastLeftOffset, rightOffset-lastLeftOffset)
                    .trimmed();
            if (queryTest.length() != 0) {
                if (queryTest != curDelim) {
                    list.append(queryTest.toString());
                }
                lastLeftOffset = i+1;
            }

        }
    }

    return list;
}

QList<SentenceTokenPtr> SentencesParser::parseToTokens(
    const QString & sentence, SentenceTokenType leftOpenType) const
{
    QList<SentenceTokenPtr> list;

    int allLen = sentence.length();
    QChar curChar;
    QChar nextChar;

    bool inString = false;// in "enclosed string" or `identifier`
    bool inLineComment = false; // in one-line comment (# or --)
    bool inBigComment = false; // in /* multi-line */ or /*! conditional comment */
    bool inEscape = false; // Previous char was backslash
    QChar lastStringEncloser = QChar::Null;

    SentenceTokenPtr curToken;

    if (leftOpenType != SentenceTokenType::None) {
        curToken.reset(new SentenceToken(0, leftOpenType));
        curToken->leftOpen = true;

        switch (leftOpenType) {
        case SentenceTokenType::MultipleLineComment:
            inBigComment = true;
            break;
        case SentenceTokenType::QuotedString:
            inString = true;
            lastStringEncloser = QChar('\'');
            break;
        case SentenceTokenType::DoubleQuotedString:
            inString = true;
            lastStringEncloser = QChar('"');
            break;
        case SentenceTokenType::QuotedIdentifier:
            inString = true;
            lastStringEncloser = QChar('`');
            break;
        default:
            break;
        }
    }

    for (int i=0; i<allLen; ++i) {
        curChar = sentence.at(i);
        nextChar = ((i+1) < allLen) ? sentence.at(i+1) : QChar::Null;

        if (!curToken) {
            curToken.reset(new SentenceToken(i, SentenceTokenType::Text));
        }

        // inLineComment
        if (!inLineComment && !inBigComment && !inString) {
            inLineComment = (curChar == QChar('#'))
                || (curChar == QChar('-') && nextChar == QChar('-'));
            if (inLineComment) { // just entered
                if (curToken) {
                    curToken->setEndIndex(i);
                    if (curToken->len > 0)
                        list.append(curToken);
                }
                curToken.reset(
                    new SentenceToken(i, SentenceTokenType::SingleLineComment));
            }
        }

        // inBigComment
        if (!inLineComment && !inBigComment && !inString) {
            if (curChar == QChar('/') && nextChar == QChar('*')) { // enter
                inBigComment = true;
                if (curToken) {
                    curToken->setEndIndex(i);
                    if (curToken->len > 0)
                        list.append(curToken);
                }
                curToken.reset(
                    new SentenceToken(i, SentenceTokenType::MultipleLineComment));
                ++i;
            }
        }
        if (inBigComment && !inLineComment && !inString) {
            if (curChar == QChar('*') && nextChar == QChar('/')) { // leave
                inBigComment = false;
                if (curToken &&
                    curToken->type == SentenceTokenType::MultipleLineComment) {
                    curToken->setEndIndex(i+2); // include both */
                    if (curToken->len > 0)
                        list.append(curToken);
                    curToken.reset();
                }
                ++i;
            }
        }

        // inString
        if (!inEscape && !inLineComment && !inBigComment) {
            if (curChar == QChar('"') || // " ' `
                curChar == QChar('\'') ||
                curChar == QChar('`')) { // str or id enclosers

                if (!inString || (inString && curChar == lastStringEncloser)) {
                    inString = !inString;

                    lastStringEncloser = curChar;
                    if (curToken) { // leaved or prev token
                        int includeEncloser = inString ? 0 : 1;
                        curToken->setEndIndex(i + includeEncloser);
                        if (curToken->len > 0)
                            list.append(curToken);
                        curToken.reset();
                    }
                    if (inString) { // just entered
                        auto tokenType = SentenceTokenType::QuotedString;
                        if (curChar == QChar('`')) {
                            tokenType = SentenceTokenType::QuotedIdentifier;
                        } else if (curChar == QChar('"')) {
                            tokenType = SentenceTokenType::DoubleQuotedString;
                        }
                        curToken.reset(new SentenceToken(i, tokenType));
                    }
                }
            }
        }

        // new lines
        if ((curChar == QChar::LineFeed && nextChar != QChar::LineFeed)) {
            inLineComment = false;
            if (curToken &&
                curToken->type == SentenceTokenType::SingleLineComment) {
                curToken->setEndIndex(i);
                if (curToken->len > 0)
                    list.append(curToken);
                curToken.reset();
            }
            // allow other types to be multiline even it may be incorrect
        }

        // inEscape
        if (!inEscape) {
            inEscape = (curChar == QChar('\\'));
        } else {
            inEscape = false;
        }

        // TODO detect identifiers started with "." e.g db.table_name
        // TODO detect numbers, (dates?) etc
        // TODO session vars & functions https://dev.mysql.com/doc/refman/5.6/en/hexadecimal-literals.html
        // TODO TRUE/FALSE/NULL https://dev.mysql.com/doc/refman/5.6/en/expressions.html
        // TODO types
    }

    if (curToken) {
        if (inBigComment || inString) {
            curToken->rightOpen = true;
        }
        curToken->setEndIndex(allLen);
        if (curToken->len > 0)
            list.append(curToken);
    }

    return list;
}

} // namespace user_query
} // namespace db
} // namespace meow
