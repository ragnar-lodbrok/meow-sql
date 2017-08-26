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

} // namespace user_query
} // namespace db
} // namespace meow
